#include <iostream>
#include <vector>
#include "DisjointForest.h"

inline float thresholdFunction(const float componentSize,const float k){
    return k/componentSize;
}

void segmentImage(const std::vector<Edge *> &edges, int totalComponents, const int minimumComponentSize, const float kValue) {
    std::cout << "Starting Segmentation:\n";
    Pixel* pixel1;
    Pixel* pixel2;
    Component* component1;
    Component* component2;
    double minimumInternalDifference;
    for(Edge* edge: edges){
        pixel1 = edge->n1;
        pixel2 = edge->n2;

        component1 = pixel1->parentTree;
        component2 = pixel2->parentTree;
        if(component1!=component2){
            minimumInternalDifference = std::min(component1->MSTMaxEdge +
                                                               thresholdFunction(component1->pixels.size(), kValue),
                                                       component2->MSTMaxEdge +
                                                               thresholdFunction(component2->pixels.size(), kValue));
            if(edge->weight < minimumInternalDifference){
                mergeComponents(component1, component2,  edge->weight);
                --totalComponents;
            }
        }
    }

    std::cout << "Segmentation Done\n";
    std::cout << "Before Post Processing Total Components: " << totalComponents << '\n';

//    post-processing:
    for(Edge* edge: edges){
        pixel1 = edge->n1;
        pixel2 = edge->n2;

        component1 = pixel1->parentTree;
        component2 = pixel2->parentTree;
        if(component1->representative!=component2->representative){
            if ((component1->pixels.size()<minimumComponentSize) || (component2->pixels.size()<minimumComponentSize)){
                mergeComponents(component1, component2, edge->weight);
                --totalComponents;
            }
        }
    }

    std::cout << "After Post Processing Total Components: " << totalComponents << '\n';
}
