//
// Created by mo on 21/05/20.
//
#ifndef STD_VECTOR
#define STD_VECTOR
#include <vector>
#endif

struct Component;
struct ComponentStruct;

struct Pixel{
    Component* parentTree;
    Pixel* parent;
    int intensity;
    int row;
    int column;
};

struct Component{
    ComponentStruct* parentComponentStruct;
    std::vector<Pixel *> pixels;
    int rank = 0;
    Pixel* representative;
    int MSTMaxEdge = 0;
};

struct Edge{
    int weight;
    Pixel* n1;
    Pixel* n2;
};


struct ComponentStruct{
    ComponentStruct* previousComponentStruct=nullptr;
    Component* component{};
    ComponentStruct* nextComponentStruct= nullptr;
};

Component* makeComponent(int row, int col, int intensity);
Edge* createEdge(Pixel* pixel1, Pixel* pixel2);
void mergeComponents(Component* x, Component* y, int MSTMaxEdgeValue);
