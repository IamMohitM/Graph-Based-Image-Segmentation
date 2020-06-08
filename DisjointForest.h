//
// Created by mo on 21/05/20.
//
#include <vector>

struct Component;
struct ComponentStruct;

struct Pixel{
    Component* parentTree;
    Pixel* parent;
    int intensity;
    int bValue;
    int gValue;
    int rValue;
    int row;
    int column;
};

struct Component{
    ComponentStruct* parentComponentStruct;
    std::vector<Pixel *> pixels;
    int rank = 0;
    Pixel* representative;
    double MSTMaxEdge = 0;
};

struct Edge{
    double weight;
    Pixel* n1;
    Pixel* n2;
};


struct ComponentStruct{
    ComponentStruct* previousComponentStruct=nullptr;
    Component* component{};
    ComponentStruct* nextComponentStruct= nullptr;
};

Component* makeComponent(int row, int col, int intensity);
Component* makeComponent(int row, int col, int b, int g, int r);
Edge* createEdge(Pixel* pixel1, Pixel* pixel2);
Edge* createRGBEdge(Pixel* pixel1, Pixel* pixel2);
void mergeComponents(Component* x, Component* y, double MSTMaxEdgeValue);
