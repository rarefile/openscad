#include "core/NodeDiff.h"

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#include "core/node.h"
#include "utils/printutils.h"

namespace NodeDiff {

namespace {

// Mixes value into seed, as boost::hash_combine does.
void combine(size_t& seed, size_t value)
{
  seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

size_t collect(const AbstractNode& node, Signatures& out)
{
  size_t seed = 0;
  // Like in the ID string, list and root nodes only group their children.
  if (!dynamic_cast<const ListNode *>(&node) && !dynamic_cast<const RootNode *>(&node)) {
    seed = std::hash<std::string>{}(STR(node));
  }
  combine(seed, node.modinst->isBackground());
  combine(seed, node.modinst->isHighlight());
  for (const auto& child : node.getChildren()) combine(seed, collect(*child, out));
  out[node.index()] = seed;
  return seed;
}

void findChanged(const AbstractNode& node, const Signatures& current,
                 const std::unordered_set<size_t>& previous, std::unordered_set<int>& out)
{
  std::vector<const AbstractNode *> changedChildren;
  for (const auto& child : node.getChildren()) {
    if (!previous.count(current.at(child->index()))) changedChildren.push_back(child.get());
  }
  if (changedChildren.empty()) {
    out.insert(node.index());
    return;
  }
  for (const auto *child : changedChildren) findChanged(*child, current, previous, out);
}

}  // namespace

Signatures signatures(const AbstractNode& root)
{
  Signatures result;
  collect(root, result);
  return result;
}

std::unordered_set<int> changedNodes(const AbstractNode& root, const Signatures& current,
                                     const Signatures& previous)
{
  std::unordered_set<size_t> known;
  known.reserve(previous.size());
  for (const auto& entry : previous) known.insert(entry.second);

  std::unordered_set<int> result;
  if (!known.count(current.at(root.index()))) findChanged(root, current, known, result);
  return result;
}

}  // namespace NodeDiff
