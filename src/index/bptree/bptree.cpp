#include "bptree.h"

#include <iostream>

using namespace std;

BPlusTree::BPlusTree(int maxKeys)
    : root_(nullptr),
      maxKeys_(maxKeys) {
}

BPlusTreeNode*
BPlusTree::findLeaf(int key) {

    BPlusTreeNode* current =
        root_;

    while(current != nullptr
          && !current->isLeaf) {

        int i = 0;

        while(i < current->keys.size()
              && key >= current->keys[i]) {

            i++;
        }

        current =
            current->children[i];
    }

    return current;
}

bool BPlusTree::search(
    int key,
    int& value
) {

    BPlusTreeNode* leaf =
        findLeaf(key);

    if(leaf == nullptr) {
        return false;
    }

    for(size_t i = 0;
        i < leaf->keys.size();
        i++) {

        if(leaf->keys[i] == key) {

            value =
                leaf->values[i];

            return true;
        }
    }

    return false;
}

void BPlusTree::insert(
    int key,
    int value
) {

    if(root_ == nullptr) {

        root_ =
            new BPlusTreeNode(true);

        root_->keys.push_back(key);

        root_->values.push_back(value);

        return;
    }

    BPlusTreeNode* leaf =
        findLeaf(key);

    auto keyIt =
        leaf->keys.begin();

    auto valueIt =
        leaf->values.begin();

    while(keyIt != leaf->keys.end()
          && *keyIt < key) {

        keyIt++;
        valueIt++;
    }

    leaf->keys.insert(
        keyIt,
        key
    );

    leaf->values.insert(
        valueIt,
        value
    );

    if(leaf->keys.size() >
       maxKeys_) {

        splitLeaf(leaf);
    }
}

void BPlusTree::splitLeaf(
    BPlusTreeNode* leaf
) {

    int split =
        leaf->keys.size() / 2;

    BPlusTreeNode* newLeaf =
        new BPlusTreeNode(true);

    for(size_t i = split;
        i < leaf->keys.size();
        i++) {

        newLeaf->keys.push_back(
            leaf->keys[i]
        );

        newLeaf->values.push_back(
            leaf->values[i]
        );
    }

    leaf->keys.resize(split);

    leaf->values.resize(split);

    newLeaf->next =
        leaf->next;

    leaf->next =
        newLeaf;

    newLeaf->parent =
        leaf->parent;

    int promoteKey =
        newLeaf->keys[0];

    insertIntoParent(
        leaf,
        promoteKey,
        newLeaf
    );
}

void BPlusTree::insertIntoParent(
    BPlusTreeNode* left,
    int key,
    BPlusTreeNode* right
) {

    if(left == root_) {

        BPlusTreeNode* newRoot =
            new BPlusTreeNode(false);

        newRoot->keys.push_back(key);

        newRoot->children.push_back(
            left
        );

        newRoot->children.push_back(
            right
        );

        left->parent =
            newRoot;

        right->parent =
            newRoot;

        root_ = newRoot;

        return;
    }

    BPlusTreeNode* parent =
        left->parent;

    int insertPos = 0;

    while(insertPos < parent->children.size()
          && parent->children[insertPos]
             != left) {

        insertPos++;
    }

    parent->keys.insert(
        parent->keys.begin()
        + insertPos,
        key
    );

    parent->children.insert(
        parent->children.begin()
        + insertPos + 1,
        right
    );

    right->parent = parent;

    if(parent->keys.size()
       > maxKeys_) {

        splitInternal(parent);
    }
}

void BPlusTree::splitInternal(
    BPlusTreeNode* node
) {

    int split =
        node->keys.size() / 2;

    int promoteKey =
        node->keys[split];

    BPlusTreeNode* newInternal =
        new BPlusTreeNode(false);

    for(size_t i = split + 1;
        i < node->keys.size();
        i++) {

        newInternal->keys.push_back(
            node->keys[i]
        );
    }

    for(size_t i = split + 1;
        i < node->children.size();
        i++) {

        newInternal->children.push_back(
            node->children[i]
        );

        node->children[i]->parent =
            newInternal;
    }

    node->keys.resize(split);

    node->children.resize(split + 1);

    insertIntoParent(
        node,
        promoteKey,
        newInternal
    );
}

void BPlusTree::printTree() {

    if(root_ == nullptr) {
        return;
    }

    vector<BPlusTreeNode*> current;

    current.push_back(root_);

    while(!current.empty()) {

        vector<BPlusTreeNode*> next;

        for(BPlusTreeNode* node
            : current) {

            cout << "[ ";

            for(int key : node->keys) {

                cout << key
                     << " ";
            }

            cout << "] ";

            if(!node->isLeaf) {

                for(BPlusTreeNode* child
                    : node->children) {

                    next.push_back(child);
                }
            }
        }

        cout << endl;

        current = next;
    }
}