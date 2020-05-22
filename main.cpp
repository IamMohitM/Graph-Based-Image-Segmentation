#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "DisjointForest.h"
#include "utils.h"

int main() {
    cv::Mat img = cv::imread("/home/mo/CLionProjects/GraphBasedImageSegmentation/images/eiffel-tower.jpg",
            0);
    std::cout << img.size() << '\n';
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


    for(int i =0; i < edgeArraySize; ++i){
        Edge* edge = edges[i];
//        std::cout << "Node 1:" << '(' << edge->n1->row << ", " << edge->n1->column <<")\n";
//        std::cout << "Node 2:" << '(' << edge->n2->row << ", " << edge->n2->column <<")\n";
//        std::cout << "Weight: " << edge->n1->intensity << " - "<<edge->n2->intensity << " = " <<edge->weight  << '\n';
    }


    return 0;
}

//    cv::imshow("Display window", img);
//    int k = cv::waitKey(0);
//     img.type();
//    std::cout << cv::typeToString(img.type()) << "\n\n";
//    std::cout << static_cast<int>(img.at<uchar>(img.rows-1, img.cols-3)) << '\n';
