#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include "DisjointForest.h"
#include "utils.h"
#include "segmentation.h"

int main(int argc, char* argv[]) {
    if (argc != 7){
        std::cout << "Execute: .\\GraphSegment inputImage outputDir color k(float) sigma(float) minSize(int)\n";
        std::cout << "Exiting program\n";
        std::exit(1);
    }
    const std::string inputPath =  argv[1];
    const std::string outputFolder = argv[2];
    const std::string color = argv[3];

    float gaussianBlur, kValue;
    int minimumComponentSize;
    std::stringstream convert;

    convert << argv[4] << " " << argv[5] << " " << argv[6];
    if (!(convert >> kValue) || !(convert >> gaussianBlur) || !(convert >> minimumComponentSize)){
        std::cout << "Execute: .\\GraphSegment inputImage outputDir color k(float) sigma(float) minSize(int)\n";
        std::cout << "Something wrong with value k, sigma or minSize, (arguments - 5, 6, 7) \n";
        std::cout << "Exiting program\n";
        std::exit(1);
    }
    printParameters(inputPath, outputFolder, color, gaussianBlur, kValue, minimumComponentSize);

    const std::filesystem::path path = std::filesystem::u8path(inputPath);
    const std::string baseFileName = getFileNameFromPath(inputPath);
    std::cout << "Filename: " << baseFileName << '\n';

    int flag;
    if (color=="rgb"){
        flag = cv::IMREAD_COLOR;
    } else{
        flag = cv::IMREAD_GRAYSCALE;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    cv::Mat img = cv::imread(path, flag);
    std::cout << img.size() << '\n';
    cv::GaussianBlur(img, img, cv::Size(3,3), gaussianBlur);

    std::cout << "After Resize: " << img.size() << '\n';

    int rows = img.rows;
    int columns = img.cols;
    std::cout << "Rows: " << rows << '\n';
    std::cout << "Columns: " << columns << '\n';
    std::vector<Pixel *> pixels(rows*columns);
    int edgeArraySize = getEdgeArraySize(rows, columns);
    std::vector<Edge *> edges(edgeArraySize);

    if (color == "rgb"){
        pixels = constructRGBImageGraph(img, pixels, rows, columns);
        edges = setRGBEdges(img, pixels, edges);
    }else{
        pixels = constructImageGraph(img, pixels, rows, columns);
        edges = setEdges(img, pixels, edges);
    }

    std::cout << "Sorting\n";
    std::sort(edges.begin(), edges.end(), compareEdges);

    int totalComponents = img.rows*img.cols;
    segmentImage(edges, totalComponents, minimumComponentSize, kValue);

    auto firstComponentStruct = pixels[0]->parentTree->parentComponentStruct;
    while(firstComponentStruct->previousComponentStruct){
        firstComponentStruct = firstComponentStruct->previousComponentStruct;
    }

    std::string outputPath = outputFolder+baseFileName+"-"+color+"-k"+
                              std::to_string(static_cast<int>(kValue))+'-'+std::to_string(gaussianBlur)+"-"
                              "min"+std::to_string(static_cast<int>(minimumComponentSize))+".jpg";

    std::filesystem::path destinationPath = std::filesystem::u8path(outputPath);
    std::cout << "Image saved as: " << outputPath << '\n';
    cv::Mat segmentedImage = addColorToSegmentation(firstComponentStruct, img.rows, img.cols);
    std::cout << segmentedImage.size() << '\n';
    std::cout << segmentedImage.rows << '\n';
    std::cout << segmentedImage.cols<< '\n';
    cv::imshow("Image", segmentedImage);
    cv::imwrite(destinationPath, segmentedImage);
    cv::waitKey(0);

    return 0;
}

//TODO: Fix relative Paths
