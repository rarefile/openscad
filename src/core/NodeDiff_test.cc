#include "core/NodeDiff.h"

#include <catch2/catch_all.hpp>
#include <functional>
#include <memory>
#include <set>
#include <string>

#include "core/BuiltinContext.h"
#include "core/Builtins.h"
#include "core/Context.h"
#include "core/EvaluationSession.h"
#include "core/SourceFile.h"
#include "core/node.h"
#include "openscad.h"

namespace {

// Parses and instantiates a design, keeping everything alive for the test.
struct Design {
  explicit Design(const std::string& code)
  {
    static const bool builtinsReady = (Builtins::initialize(), true);
    (void)builtinsReady;

    SourceFile *parsed = nullptr;
    REQUIRE(parse(parsed, code, "test.scad", "test.scad", false));
    file.reset(parsed);
    session = std::make_unique<EvaluationSession>(".");
    ContextHandle<BuiltinContext> builtin_context{Context::create<BuiltinContext>(session.get())};
    AbstractNode::resetIndexCounter();
    std::shared_ptr<const FileContext> file_context;
    root = file->instantiate(*builtin_context, &file_context);
    REQUIRE(root);
  }

  // Names of the nodes with the given indices, e.g. {"cube", "translate"}.
  std::multiset<std::string> names(const std::unordered_set<int>& indices) const
  {
    std::multiset<std::string> result;
    std::function<void(const AbstractNode&)> walk = [&](const AbstractNode& node) {
      if (indices.count(node.index())) result.insert(node.verbose_name());
      for (const auto& child : node.getChildren()) walk(*child);
    };
    walk(*root);
    return result;
  }

  std::unique_ptr<SourceFile> file;
  std::unique_ptr<EvaluationSession> session;
  std::shared_ptr<AbstractNode> root;
};

std::multiset<std::string> changedNames(const std::string& before, const std::string& after)
{
  const Design oldDesign(before);
  const auto previous = NodeDiff::signatures(*oldDesign.root);
  const Design newDesign(after);
  const auto current = NodeDiff::signatures(*newDesign.root);
  return newDesign.names(NodeDiff::changedNodes(*newDesign.root, current, previous));
}

}  // namespace

TEST_CASE("NodeDiff reports nothing for an unchanged design", "[core][NodeDiff]")
{
  const std::string code = "w = 10; cube(w); translate([20, 0, 0]) sphere(5);";
  CHECK(changedNames(code, code).empty());
}

TEST_CASE("NodeDiff reports only the primitive whose size changed", "[core][NodeDiff]")
{
  CHECK(changedNames("w = 10; cube(w); translate([20, 0, 0]) sphere(5);",
                     "w = 12; cube(w); translate([20, 0, 0]) sphere(5);") ==
        std::multiset<std::string>{"cube"});
}

TEST_CASE("NodeDiff reports a moved object as its whole transform", "[core][NodeDiff]")
{
  CHECK(changedNames("x = 20; cube(10); translate([x, 0, 0]) sphere(5);",
                     "x = 25; cube(10); translate([x, 0, 0]) sphere(5);") ==
        std::multiset<std::string>{"translate"});
}

TEST_CASE("NodeDiff follows a value through modules", "[core][NodeDiff]")
{
  const std::string module = "module post(h) { cylinder(h = h, r = 1); } ";
  CHECK(changedNames(module + "a = 5; b = 8; post(a); translate([5, 0, 0]) post(b);",
                     module + "a = 5; b = 9; post(a); translate([5, 0, 0]) post(b);") ==
        std::multiset<std::string>{"cylinder"});
}

TEST_CASE("NodeDiff tells apart identical objects when one changes", "[core][NodeDiff]")
{
  CHECK(changedNames("a = 5; b = 5; cube(a); translate([10, 0, 0]) cube(b);",
                     "a = 5; b = 6; cube(a); translate([10, 0, 0]) cube(b);") ==
        std::multiset<std::string>{"cube"});
}

TEST_CASE("NodeDiff reports objects added by a loop", "[core][NodeDiff]")
{
  CHECK(changedNames("n = 2; for (i = [0 : n - 1]) translate([i * 3, 0, 0]) cube(1);",
                     "n = 3; for (i = [0 : n - 1]) translate([i * 3, 0, 0]) cube(1);") ==
        std::multiset<std::string>{"translate"});
}
