#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "DisjointForest.h"


int getSingleIndex(const int row, const int col, const int totalColumns){
    return (row*totalColumns) + col;
}

void printParameters(const std::string &inputPath, const std::string &outputDir, const std::string &color,
        const float sigma, const float k, const int minimumComponentSize){
    std::cout << "Input Path: " << inputPath << '\n';
    std::cout << "Output Directory: " << outputDir << '\n';
    std::cout << "Color Space: " << color << '\n';
    std::cout << "Sigma: " << sigma << '\n';
    std::cout << "k: " << k << '\n';
    std::cout << "Minimum Component Size: " << minimumComponentSize << '\n';
}

std::vector<std::string> split(const std::string& s, const char separator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

std::string getFileNameFromPath(const std::string &path){
    std::vector<std::string> pathSplit = split(path, '/');
    std::string fileName = pathSplit[pathSplit.size()-1];
    std::vector<std::string> fileNameSplit = split(fileName, '.');
    std::string baseFileName = fileNameSplit[0];
    return baseFileName;
}

int getEdgeArraySize(const int rows,const int columns){
    int firstColumn = 3 * (rows-1);
    int lastColumn = 2 * (rows - 1);
    int middleValues = 4 * (rows - 1 ) * (columns - 2);
    int lastRow = columns - 1;
    return firstColumn + lastColumn + middleValues + lastRow;
}

std::vector<Pixel *> constructImagePixels(const cv::Mat &img, int rows, int columns){
    std::vector<Pixel *> pixels(rows*columns);

    Component* firstComponent = makeComponent(0, 0, img.at<cv::Vec3b>(0, 0));
    auto* firstComponentStruct = new ComponentStruct;
    firstComponentStruct->component = firstComponent;
    auto previousComponentStruct = firstComponentStruct;
    int index;

    for(int row=0; row < rows; row++)
    {
        for(int column=0; column < columns; column++)
        {
            Component* component=makeComponent(row, column, img.at<cv::Vec3b>(row, column));
            auto* newComponentStruct = new ComponentStruct;
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
    return pixels;
}

std::vector<Edge *> setEdges(const std::vector<Pixel *> &pixels, const std::string colorSpace, const int rows, const int columns){
    int edgeArraySize = getEdgeArraySize(rows, columns);
    std::vector<Edge *> edges(edgeArraySize);
    std::function<double(Pixel*, Pixel*)> edgeDifferenceFunction;
    if (colorSpace == "rgb"){
        edgeDifferenceFunction = rgbPixelDifference;
    }else{
        edgeDifferenceFunction = grayPixelDifference;
    }
    int edgeCount = 0;
    for(int row=0; row < rows; ++row){
        for(int column=0; column < columns; ++column) {
            Pixel* presentPixel = pixels[getSingleIndex(row, column, columns)];
            if(row < rows - 1){
                if(column == 0){
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column+1, columns)], edgeDifferenceFunction);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column+1, columns)], edgeDifferenceFunction);
                    edges[edgeCount++] = createEdge(presentPixel , pixels[getSingleIndex(row+1, column, columns)], edgeDifferenceFunction);
                }
                else if(column==columns-1){
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column, columns)], edgeDifferenceFunction);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column-1, columns)], edgeDifferenceFunction);
                }else{
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column+1, columns)], edgeDifferenceFunction);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column+1, columns)], edgeDifferenceFunction);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column, columns)], edgeDifferenceFunction);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column-1, columns)], edgeDifferenceFunction);
                }
            }
            else if(row == rows - 1){
                if(column != columns - 1) {
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row,column+1, columns)], edgeDifferenceFunction);
                }
            }
        }
    }
    std::cout << "Total Edges: "<< edgeCount << '\n';
    return edges;
}

bool compareEdges(const Edge* e1, const Edge* e2){
    return e1->weight < e2->weight;
}

int getRandomNumber(const int min,const int max)
{
    //from learncpp.com
    static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}


cv::Mat addColorToSegmentation(const ComponentStruct* componentStruct, int rows, int columns){
    cv::Mat segmentedImage(rows, columns, CV_8UC3);
    do{
        uchar r=getRandomNumber(0, 255);
        uchar b=getRandomNumber(0, 255);
        uchar g=getRandomNumber(0, 255);
        cv::Vec3b pixelColor= {b ,g ,r};
        for(auto pixel: componentStruct->component->pixels){
            segmentedImage.at<cv::Vec3b>(cv::Point(pixel->column,pixel->row)) = pixelColor;
        }
        componentStruct = componentStruct->nextComponentStruct;
    }while(componentStruct);
    
    return segmentedImage;
}