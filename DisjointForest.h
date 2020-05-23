//
// Created by mo on 21/05/20.
//
#ifndef STD_VECTOR
#define STD_VECTOR
#include <vector>
#endif

struct Component;

struct Pixel{
    Component* parentTree;
    Pixel* parent;
    int intensity;
    int row;
    int column;
};

struct Component{
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

Component* makeComponent(int row, int col, int intensity);
inline Component* findSet(Pixel* x);
void link(Component* x, Component* y, std::vector<Component *> &trees, int MSTMaxEdgeValue);
void setUnion(Pixel* x, Pixel* y, std::vector<Component *> &trees, int MSTMaxEdgeValue);
Edge* createEdge(Pixel* pixel1, Pixel* pixel2);
