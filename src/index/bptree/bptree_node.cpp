#include "bptree_node.h"

using namespace std;

BPlusTreeNode::BPlusTreeNode(bool leaf)
    : isLeaf(leaf),
      parent(nullptr),
      next(nullptr) {
}