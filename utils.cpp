#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdlib>
#include "DisjointForest.h"
#include <string>

int getSingleIndex(const int row,const int col,const int totalColumns){
    return (row*totalColumns) + col;
}

std::vector<std::string> split(const std::string& s, char seperator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

int getEdgeArraySize(int rows, int columns){
    int firstColumn = 3 * (rows-1);
    int lastColumn = 2 * (rows - 1);
    int middleValues = 4 * (rows - 1 ) * (columns - 2);
    int lastRow = columns - 1;
    return firstColumn + lastColumn + middleValues + lastRow;
}

void setEdges(const cv::Mat &img, std::vector<Pixel *> &pixels, std::vector<Edge*> &edges){
    int rows = img.rows;
    int columns = img.cols;
    int edgeCount = 0;
    for(int row=0; row < img.rows; ++row){
        for(int column=0; column < img.cols; ++column) {
            Pixel* presentPixel = pixels[getSingleIndex(row, column, columns)];
            if(row < rows - 1){
                if(column == 0){
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column+1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column+1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel , pixels[getSingleIndex(row+1, column, columns)]);
                }
                else if(column==columns-1){
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column-1, columns)]);
                }else{
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column+1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column+1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row+1, column-1, columns)]);
                }
            }
            else if(row == rows - 1){
                if(column != columns - 1) {
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row,column+1, columns)]);
                }
            }
        }
    }
    std::cout << "Total Edges: "<< edgeCount << '\n';
}


int returnMedian(Edge** &edges, int x, int y, int z){
    if(((edges[y]->weight > edges[x]->weight) && (edges[x]->weight > edges[z]->weight)) ||
                                                        ((edges[y]->weight < edges[x]->weight) && ( edges[x]->weight < edges[z]->weight))){
        return x;
    }
    else if(((edges[x]->weight > edges[y]->weight) && (edges[y]->weight > edges[z]->weight)) ||
                                                        ((edges[x]->weight < edges[y]->weight) && ( edges[y]->weight < edges[z]->weight))){
        return y;
    }else{
        return z;
    }
}

int partition(Edge** &edges, int startingIndex, int lastIndex, int &count){
    count += lastIndex-startingIndex;
//    std::cout << "Starting Index: " << startingIndex << ' ' << "Last Index: " << lastIndex << '\n';
    int pivot = edges[startingIndex]->weight;
    int pivotIndex = startingIndex+1;
    for (int i=startingIndex+1; i < lastIndex; ++i){
        if (edges[i]->weight < pivot){
            std::swap(edges[pivotIndex], edges[i]);
            ++pivotIndex;
        }
    }
    --pivotIndex;
    std::swap(edges[pivotIndex], edges[startingIndex]);
    return pivotIndex;
}

void quickSort(Edge** &edges, int startingIndex, int lastIndex, int &count){
    if (startingIndex < lastIndex){

        int centerElement = (startingIndex + lastIndex)/2;
        int medianElement = returnMedian(edges, startingIndex, centerElement, lastIndex-1);
        std::swap(edges[startingIndex], edges[medianElement]);

        int pivotIndex = partition(edges, startingIndex, lastIndex, count);
        quickSort(edges, startingIndex, pivotIndex, count);
        quickSort(edges, pivotIndex+1, lastIndex, count);
    }
}

int getRandomNumber(int min, int max)
{
    //from learncpp.com
    static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}


cv::Mat addColorToSegmentation(ComponentStruct* componentStruct, int rows, int columns){
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

bool compareEdges(Edge* e1, Edge* e2){
    return e1->weight < e2->weight;
}