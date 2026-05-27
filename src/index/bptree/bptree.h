#pragma once

#include "bptree_node.h"

using namespace std;

class BPlusTree {
private:
    BPlusTreeNode* root_;

    int maxKeys_;

    BPlusTreeNode* findLeaf(int key);

    void splitLeaf(
        BPlusTreeNode* leaf
    );

    void splitInternal(
        BPlusTreeNode* node
    );

    void insertIntoParent(
        BPlusTreeNode* left,
        int key,
        BPlusTreeNode* right
    );

public:
    explicit BPlusTree(int maxKeys);

    void insert(int key, int value);

    bool search(int key, int& value);

    void printTree();
};