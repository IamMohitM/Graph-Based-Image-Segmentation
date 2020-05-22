#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "DisjointForest.h"
#include "utils.h"

int main() {
    cv::Mat img = cv::imread("/home/mo/CLionProjects/GraphBasedImageSegmentation/images/eiffel-tower.jpg",
            0);
    std::cout << img.size() << '\n';
    cv::resize(img, img, cv::Size(256, 256));
    std::cout << "After Resize: " << img.size() << '\n';

    std::vector<Pixel *> pixels(img.rows*img.cols);
    std::vector<Component *> allComponents(img.rows * img.cols);
    int rows = img.rows;
    int columns = img.cols;
    int index;

    for(int row=0; row < rows; row++)
    {
        for(int column=0; column < columns; column++)
        {
            int pixelValue = static_cast<int>(img.at<uchar>(row, column));
            Component* component=makeComponent(row, column, pixelValue);
            index = getSingleIndex(row, column, columns);
            pixels[index] = component->pixels.at(0);
            allComponents[index] = component;
        }
    }
    std::cout << img.rows * img.cols << '\n';
    std::cout << pixels.size() << '\n';
    std::cout << allComponents.size() << '\n';

    std::cout << checkValidityOfGraph(rows, columns, img, pixels) << '\n';
    auto [edges, edgeArraySize] = getEdges(img, pixels);
    for(int i=0; i < edgeArraySize; ++i){
        std::cout << edges[i]->weight << '\n';
    }
    std::cout << "Sorting\n";
    quickSort(edges, 0, edgeArraySize);
    for(int i=0; i < edgeArraySize; ++i){
        std::cout << edges[i]->weight << '\n';
    }

    return 0;
}

