//
// Created by mo on 21/05/20.
//
#include "DisjointForest.h"
#include <iostream>
#include <cmath>

Component* makeComponent(const int row, const int column, const int intensity){
    auto* component = new Component;
    auto* pixel = new Pixel;
    pixel->intensity = intensity;
    pixel->row = row;
    pixel->column = column;
    pixel->parent = pixel;
    pixel->parentTree = component;
    component->representative = pixel;
    component->pixels.push_back(pixel);
    return component;
}

Component* makeComponent(const int row, const int column, const int b,const int g,const int r){
    auto* component = new Component;
    auto* pixel = new Pixel;
    pixel->bValue = b;
    pixel->gValue = g;
    pixel->rValue = r;
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

double rgbPixelDifference(const int b1,const int g1,const int r1,const int b2,const int g2,const int r2){
    return sqrt(pow(r1- r2, 2 ) +
                pow(b1-b2, 2) +
                pow(g1-g2, 2));
}

Edge* createEdge(Pixel* pixel1, Pixel* pixel2){
    Edge* edge = new Edge;
    edge->weight = abs(pixel1->intensity - pixel2 ->intensity);
    edge->n1 = pixel1;
    edge->n2 = pixel2;
    return edge;
}


Edge* createRGBEdge(Pixel* pixel1, Pixel* pixel2){
    Edge* edge = new Edge;
    edge->weight = rgbPixelDifference(pixel1->bValue, pixel1->gValue, pixel1->rValue, pixel2->bValue,
                                      pixel2->gValue, pixel2->rValue);
    edge->n1 = pixel1;
    edge->n2 = pixel2;
    return edge;
}

void mergeComponents(Component* x, Component* y,const double MSTMaxEdgeValue){
    if (x != y) {
        ComponentStruct* componentStruct;
        if (x->rank < y->rank) {
            x->representative->parent = y->representative;
            y->MSTMaxEdge = MSTMaxEdgeValue;
            setParentTree(x, y);
            componentStruct = x->parentComponentStruct;
            delete x;
        } else {
            if (x->rank == y->rank) {
                ++x->rank;
            }
            y->representative->parent = x->representative->parent;
            x->MSTMaxEdge = MSTMaxEdgeValue;
            setParentTree(y, x);
            componentStruct = y->parentComponentStruct;
            delete y;
        }
        if(componentStruct->previousComponentStruct){
            componentStruct->previousComponentStruct->nextComponentStruct = componentStruct->nextComponentStruct;
        }
        if(componentStruct->nextComponentStruct){
            componentStruct->nextComponentStruct->previousComponentStruct = componentStruct->previousComponentStruct;
        }
        delete componentStruct;
    }
}