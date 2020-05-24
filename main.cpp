#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include "DisjointForest.h"
#include "utils.h"
#include "segmentation.h"

int main() {
    std::string path =  "/home/mo/CLionProjects/GraphBasedImageSegmentation/images/view.jpg";
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    cv::Mat img = cv::imread(path,
            0);
    std::cout << img.size() << '\n';
    cv::resize(img, img, cv::Size(256, 256));
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
            int pixelValue = static_cast<int>(img.at<uchar>(row, column));
            Component* component=makeComponent(row, column, pixelValue);
            index = getSingleIndex(row, column, columns);
            pixels[index] = component->pixels.at(0);
            allComponents[index] = component;
        }
    }
    std::cout << img.rows * img.cols << '\n';
    std::cout << "Before Segmentation total pixels: " << pixels.size() << '\n';
    std::cout << allComponents.size() << '\n';

    std::cout << checkValidityOfGraph(rows, columns, img, pixels) << '\n';
    auto [edges, edgeArraySize] = getEdges(img, pixels);

    std::cout << "Sorting\n";
    int count = 0;
    quickSort(edges, 0, edgeArraySize, count);
    std::cout << "Total comparisons: " << count << '\n';

    segmentImage(edges, allComponents, edgeArraySize);

    int totalPixels {};
    for(auto component: allComponents){
        totalPixels += component->pixels.size();
    }

    std::cout << "After Segmentation total pixels: " << totalPixels << '\n';

    int trees = 0;
    cv::Mat segmentedImage(256, 256, CV_8UC3);
    for(auto component: allComponents){
        uchar r=getRandomNumber(0, 255);
        uchar b=getRandomNumber(0, 255);
        uchar g=getRandomNumber(0, 255);
        cv::Vec3b pixelColor= {b ,g ,r};
        for(auto pixel: component->pixels){
            segmentedImage.at<cv::Vec3b>(cv::Point(pixel->column,pixel->row)) = pixelColor;
        }
        ++trees;
        std::cout << "Segmented " << trees << " components\n";
    }
    cv::Mat segmentedMat(segmentedImage);
    cv::resize(segmentedMat, segmentedMat, cv::Size(img.rows, img.cols));
    std::cout << segmentedMat << '\n';
    std::cout << segmentedMat.size() << '\n';
    cv::imwrite("/home/mo/CLionProjects/GraphBasedImageSegmentation/Results/view-k_700.jpg", segmentedImage);
    cv::imshow("Image", segmentedMat);
    cv::waitKey(0);

    for(int i=0; i < edgeArraySize; ++i){
        delete edges[i];
    }
    delete edges;
    return 0;
}

//TODO: Fix relative Paths
