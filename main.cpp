#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "DisjointForest.h"
#include "utils.h"
#include "segmentation.h"

int main() {
    std::string path =  "/home/mo/CLionProjects/GraphBasedImageSegmentation/images/eiffel-tower.jpg";
    srand(static_cast<unsigned int>(std::time(nullptr)));
    cv::Mat img = cv::imread(path, 1);
    std::cout << img.channels() << '\n';
    std::cout << img.size() << '\n';
    cv::resize(img, img, cv::Size(320, 240));
    cv::GaussianBlur(img, img, cv::Size(3,3), 0.8);

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
            cv::Vec3b pixelValue = img.at<cv::Vec3b>(row, column);
            Component* component=makeComponent(row, column, pixelValue.val[0],
                                               pixelValue.val[1],
                                               pixelValue.val[2]);
            index = getSingleIndex(row, column, columns);
            pixels[index] = component->pixels.at(0);
            allComponents[index] = component;
        }
    }
    std::cout << img.rows * img.cols << '\n';
    std::cout << "Before Segmentation total pixels: " << pixels.size() << '\n';
    std::cout << allComponents.size() << '\n';

//    std::cout << checkValidityOfGraph(rows, columns, img, pixels) << '\n';
    auto [edges, edgeArraySize] = getEdges(img, pixels);

//
    std::cout << "Sorting\n";
    int count = 0;
    quickSort(edges, 0, edgeArraySize, count);
    std:: cout << "Total Comparisons: " << count << '\n';

//    Edge** sortedEdges = countSort(edges, edgeArraySize, 255);
//    delete[] edges;
    segmentImage(edges, allComponents, edgeArraySize);
    cv::Mat segmentedImage = addColorToSegmentation(allComponents, img.rows, img.cols);
    cv::imwrite("/home/mo/CLionProjects/GraphBasedImageSegmentation/Results/eiffel-tower-rgb-k700-min20.jpg", segmentedImage);
    cv::imshow("Image", segmentedImage);
    cv::waitKey(0);
//
//
    for(int i=0; i < edgeArraySize; ++i){
        delete edges[i];
    }
    delete[] edges;

    return 0;
}

//TODO: Fix relative Paths
