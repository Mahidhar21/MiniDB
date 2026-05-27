#pragma once

#include <vector>

using namespace std;

class BPlusTreeNode {
public:
    bool isLeaf;

    vector<int> keys;

    vector<int> values;

    vector<BPlusTreeNode*> children;

    BPlusTreeNode* parent;

    BPlusTreeNode* next;

    explicit BPlusTreeNode(bool leaf);
};