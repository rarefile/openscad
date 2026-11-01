#pragma once

#include <cstddef>
#include <unordered_map>
#include <unordered_set>

class AbstractNode;

/*!
   Compares a node tree against an earlier evaluation of the same design, to
   find out which parts of the model an edit changed.

   Every subtree is identified by a signature: a hash of the node's own
   description and modifiers, combined with the signatures of its children.
   Identical subtrees therefore compare equal regardless of node indices or
   where in the source they come from, and every node is hashed only once.
 */
namespace NodeDiff {

// Signature of every subtree, by the index of its root node.
using Signatures = std::unordered_map<int, size_t>;

// Signatures of every subtree below (and including) root.
Signatures signatures(const AbstractNode& root);

// Indices of the smallest subtrees below root that do not appear in previous.
// current must be signatures(root). A changed node is reported as a whole when
// none of its children changed (e.g. a translate() whose offset changed);
// otherwise only its changed children are reported.
std::unordered_set<int> changedNodes(const AbstractNode& root, const Signatures& current,
                                     const Signatures& previous);

}  // namespace NodeDiff
