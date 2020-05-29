#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include "DisjointForest.h"
#include "utils.h"
#include "segmentation.h"
#include <algorithm>

int main() {
    float gaussianBlur = 0.8;
    int minimumComponentSize = 100;
    float kValue = 750;
    std::string inputPath =  "/home/mo/CLionProjects/GraphBasedImageSegmentation/images/eiffel-tower.jpg";
    std::string outputFolder = "/home/mo/CLionProjects/GraphBasedImageSegmentation/Results/";
    std::string baseFileName = getFileNameFromPath(inputPath);
    std::cout << "Filename: " << baseFileName << '\n';

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    cv::Mat img = cv::imread(inputPath, 0);
    std::cout << img.size() << '\n';
    cv::GaussianBlur(img, img, cv::Size(5,5), gaussianBlur);

    std::cout << "After Resize: " << img.size() << '\n';

    std::vector<Pixel *> pixels(img.rows*img.cols);
    int rows = img.rows;
    int columns = img.cols;
    int edgeArraySize = getEdgeArraySize(rows, columns);
    std::vector<Edge *> edges(edgeArraySize);
    int index;

    Component* firstComponent = makeComponent(0, 0, static_cast<int>(img.at<uchar>(0, 0)));
    auto* firstComponentStruct = new ComponentStruct;
    firstComponentStruct->component = firstComponent;
    auto previousComponentStruct = firstComponentStruct;

    for(int row=0; row < rows; row++)
    {
        for(int column=0; column < columns; column++)
        {
            int pixelValue = static_cast<int>(img.at<uchar>(row, column));
            Component* component=makeComponent(row, column, pixelValue);
            auto* newComponentStruct = new ComponentStruct;// = new ComponentStruct;
            newComponentStruct->component = component;
            newComponentStruct->previousComponentStruct = previousComponentStruct;
            previousComponentStruct->nextComponentStruct = newComponentStruct;
            component->parentComponentStruct = newComponentStruct;
            previousComponentStruct = newComponentStruct;
            index = getSingleIndex(row, column, columns);
            pixels[index] = component->pixels.at(0);
        }
    }
    firstComponentStruct = firstComponentStruct->nextComponentStruct;
    delete firstComponentStruct->previousComponentStruct;
    firstComponentStruct->previousComponentStruct = nullptr;

    int totalComponents = img.rows*img.cols;
    std::cout << "Before Segmentation total pixels: " << pixels.size() << '\n';
    setEdges(img, pixels, edges);

    std::cout << "Sorting\n";
    std::sort(edges.begin(), edges.end(), compareEdges);

    segmentImage(edges, totalComponents, minimumComponentSize, kValue);
    firstComponentStruct = pixels[0]->parentTree->parentComponentStruct;
    while(firstComponentStruct->previousComponentStruct){
        firstComponentStruct = firstComponentStruct->previousComponentStruct;
    }

    std::string outputPath = outputFolder+baseFileName+"-k"+
                              std::to_string(static_cast<int>(kValue))+'-'+std::to_string(gaussianBlur)+"-"
                              "min"+std::to_string(static_cast<int>(minimumComponentSize))+".jpg";
    std::cout << "Image saved as: " << outputPath << '\n';
    cv::Mat segmentedImage = addColorToSegmentation(firstComponentStruct, img.rows, img.cols);
    cv::imshow("Image", segmentedImage);
    cv::imwrite(outputPath, segmentedImage);
    cv::waitKey(0);

    return 0;
}

//TODO: Fix relative Paths
