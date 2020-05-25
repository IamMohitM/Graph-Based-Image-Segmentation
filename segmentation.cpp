#include <iostream>
#include <vector>
#include "DisjointForest.h"

float thresholdFunction(float componentSize){
    float k = 300;
    return k/componentSize;
}

void segmentImage(Edge** &edges, std::vector<Component *> &components, int edgeArraySize){
    int minimumComponentSize = 20;
    int totalComponents = components.size();
    std::cout << "Starting Segmentation:\n";
    for(int index =0; index < edgeArraySize; ++index){
        Pixel* pixel1 = edges[index]->n1;
        Pixel* pixel2 = edges[index]->n2;

        Component* component1 = findSet(pixel1);
        Component* component2 = findSet(pixel2);
        if(component1!=component2){
            float minimumInternalDifference = std::min(component1->MSTMaxEdge +
                                                               thresholdFunction(component1->pixels.size()),
                                                       component2->MSTMaxEdge +
                                                               thresholdFunction(component2->pixels.size()));
            if(edges[index]->weight < minimumInternalDifference){
                setUnion(pixel1, pixel2, components, edges[index]->weight);
            }
        }
    }

    std::cout << "Before Segmentation Total Components: " << totalComponents << '\n';
    std::cout << "Segmentation Done\n";
    std::cout << "Before Post Processing Total Components: " << components.size() << '\n';

    //post-processing:
    for(int index =0; index < edgeArraySize; ++index){
        Pixel* pixel1 = edges[index]->n1;
        Pixel* pixel2 = edges[index]->n2;

        Component* component1 = findSet(pixel1);
        Component* component2 = findSet(pixel2);
        if((component1!=component2)&&(component1->pixels.size()<minimumComponentSize) ||
                                        component2->pixels.size()<minimumComponentSize){
            link(component1, component2, components, edges[index]->weight);
        }
    }


    std::cout << "After Post Processing Total Components: " << components.size() << '\n';
}
