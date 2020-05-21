//
// Created by mo on 21/05/20.
//
#include "DisjointForest.h"

Component* makeSet(const int x){
    auto* tree = new Component;
    Pixel* node = new Pixel;
    node->value = x;
    tree->nodes.push_back(node);
    tree->id = node->value;
    tree->representative = node;
    node->parent = node;
    node->parentTree = tree;
    return tree;
}

inline Component* findSet(Pixel* node){
    return node->parentTree;
}

void setParentTree(Component* childTreePointer, Component* parentTreePointer){
    for(Pixel* nodePointer: childTreePointer->nodes){
        parentTreePointer->nodes.push_back(nodePointer);
        nodePointer->parentTree = parentTreePointer;
    }
}

void removeTree(std::vector<Component *> &trees, Component*  tree){
    for(int index=0; index < trees.size(); ++index){
        if(trees[index]==tree){
            trees.erase(trees.begin() + (index));
            break;
        }
    }
    delete tree;
}

void link(Component* x, Component* y, std::vector<Component *> &trees){
    if (x != y) {
        if (x->rank < y->rank) {
            x->representative->parent = y->representative;
            setParentTree(x, y);
            removeTree(trees, x);
        } else {
            if (x->rank == y->rank) {
                ++x->rank;
            }
            y->representative->parent = x->representative->parent;
            setParentTree(y, x);
            removeTree(trees, y);
        }
    }
}

void setUnion(Pixel* x, Pixel* y, std::vector<Component *> &trees){
    link(findSet(x), findSet(y), trees);
}
