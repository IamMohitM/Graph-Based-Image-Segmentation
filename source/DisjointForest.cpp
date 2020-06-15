//
// Created by mo on 21/05/20.
//
#include "DisjointForest.h"
#include <iostream>
#include <cmath>


Component* makeComponent(const int row, const int column, cv::Vec3b pixelValues){
    auto* component = new Component;
    auto* pixel = new Pixel;
    pixel->bValue = pixelValues.val[0];
    pixel->gValue = pixelValues.val[1];
    pixel->rValue = pixelValues.val[2];
    pixel->intensity = (pixelValues.val[0] + pixelValues.val[1] + pixelValues.val[2])/3;
    pixel->row = row;
    pixel->column = column;
    pixel->parent = pixel;
    pixel->parentTree = component;
    component->representative = pixel;
    component->pixels.push_back(pixel);
    return component;
}

void setParentTree(Component* childTreePointer, Component* parentTreePointer){
    for(Pixel* nodePointer: childTreePointer->pixels){
        parentTreePointer->pixels.push_back(nodePointer);
        nodePointer->parentTree = parentTreePointer;
    }
}

double grayPixelDifference(Pixel* pixel1, Pixel* pixel2){
    return abs(pixel1->intensity - pixel2->intensity);
}

double rgbPixelDifference(Pixel* pixel1, Pixel* pixel2){
    return sqrt(pow(pixel1->rValue- pixel2->rValue, 2 ) +
                pow(pixel1->bValue- pixel2->bValue, 2) +
                pow(pixel1->gValue- pixel2->gValue, 2));
}

Edge* createEdge(Pixel* pixel1, Pixel* pixel2,  std::function<double(Pixel*, Pixel*)> edgeDifferenceFunction){
    Edge* edge = new Edge;
    edge->weight = edgeDifferenceFunction(pixel1, pixel2);
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