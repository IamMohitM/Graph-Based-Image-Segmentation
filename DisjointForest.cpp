//
// Created by mo on 21/05/20.
//
#include "DisjointForest.h"
#include <iostream>
#include <cmath>

Component* makeComponent(const int row, const int column, const int bValue, const int gValue, const int rValue){
    auto* component = new Component;
    auto* pixel = new Pixel;
    pixel->bValue = bValue;
    pixel->gValue = gValue;
    pixel->rValue = rValue;
    pixel->row = row;
    pixel->column = column;
    pixel->parent = pixel;
    pixel->parentTree = component;
    component->representative = pixel;
    component->pixels.push_back(pixel);
    return component;
}

inline Component* findSet(Pixel* node){
    return node->parentTree;
}

void setParentTree(Component* childTreePointer, Component* parentTreePointer){
    for(Pixel* nodePointer: childTreePointer->pixels){
        parentTreePointer->pixels.push_back(nodePointer);
        nodePointer->parentTree = parentTreePointer;
    }
}

void removeTree(std::vector<Component *> &allComponents, Component*  component){
    for(int index=0; index < allComponents.size(); ++index){
        if(allComponents[index] == component){
            allComponents.erase(allComponents.begin() + (index));
            break;
        }
    }
    delete component;
}

void link(Component* x, Component* y, std::vector<Component *> &trees, int MSTMaxEdgeValue){
    if (x != y) {
        if (x->rank < y->rank) {
            x->representative->parent = y->representative;
            y->MSTMaxEdge = MSTMaxEdgeValue;
            setParentTree(x, y);
            removeTree(trees, x);
        } else {
            if (x->rank == y->rank) {
                ++x->rank;
            }
            y->representative->parent = x->representative->parent;
            x->MSTMaxEdge = MSTMaxEdgeValue;
            setParentTree(y, x);
            removeTree(trees, y);
        }
    }
}

void setUnion(Pixel* x, Pixel* y, std::vector<Component *> &trees, int MSTMaxEdgeValue){
    link(findSet(x), findSet(y), trees, MSTMaxEdgeValue);
}

double rgbPixelDifference( int b1, int g1, int r1, int b2, int g2, int r2){
    return sqrt(pow(r1- r2, 2 ) +
                   pow(b1-b2, 2) +
                   pow(g1-g2, 2));
}

Edge* createEdge(Pixel* pixel1, Pixel* pixel2){
    Edge* edge = new Edge;
    edge->weight = rgbPixelDifference(pixel1->bValue, pixel1->gValue, pixel1->rValue, pixel2->bValue,
                       pixel2->gValue, pixel2->rValue);
    edge->n1 = pixel1;
    edge->n2 = pixel2;
    return edge;
}
