#include <iostream>
#include <vector>
#include "DisjointForest.h"

inline float thresholdFunction(float componentSize, float k){
    return k/componentSize;
}

void segmentImage(std::vector<Edge *> &edges, int &totalComponents, int minimumComponentSize, float kValue) {
    std::cout << "Starting Segmentation:\n";
    Pixel* pixel1;
    Pixel* pixel2;
    Component* component1;
    Component* component2;
    for(int index =0; index < edges.size(); ++index){
        pixel1 = edges[index]->n1;
        pixel2 = edges[index]->n2;

        component1 = pixel1->parentTree;
        component2 = pixel2->parentTree;
        if(component1!=component2){
            float minimumInternalDifference = std::min(component1->MSTMaxEdge +
                                                               thresholdFunction(component1->pixels.size(), kValue),
                                                       component2->MSTMaxEdge +
                                                               thresholdFunction(component2->pixels.size(), kValue));
            if(edges[index]->weight < minimumInternalDifference){
                mergeComponents(component1, component2,  edges[index]->weight);
                --totalComponents;
            }
        }
    }

    std::cout << "Segmentation Done\n";
    std::cout << "Before Post Processing Total Components: " << totalComponents << '\n';

//    post-processing:
    for(int index =0; index < edges.size(); ++index){
        pixel1 = edges[index]->n1;
        pixel2 = edges[index]->n2;

        component1 = pixel1->parentTree;
        component2 = pixel2->parentTree;
        if((component1!=component2)&&(component1->pixels.size()<minimumComponentSize) ||
                                        component2->pixels.size()<minimumComponentSize){
            mergeComponents(component1, component2, edges[index]->weight);
            --totalComponents;
        }
    }

    std::cout << "After Post Processing Total Components: " << totalComponents << '\n';
}
