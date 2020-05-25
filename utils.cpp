#include <opencv2/opencv.hpp>
#include <vector>
#include <tuple>
#include <cstdlib>
#include "DisjointForest.h"

int getSingleIndex(const int row,const int col,const int totalColumns){
    return (row*totalColumns) + col;
}

bool checkValidityOfGraph(const int rows, const int cols, const cv::Mat &img, const std::vector<Pixel *> &pixels){
    int count = 0;
    for(int row=0; row < rows; row++)
    {
        for(int column=0; column < cols; column++)
        {
            int pixelValue = static_cast<int>(img.at<uchar>(row, column));
            if(pixels[getSingleIndex(row, column, cols)]->intensity == pixelValue){
                ++count;
            }
            else{
                std::cout << "Something might have gone wrong\n";
            }
        }
    }
    return count==(rows*cols);
}

int getEdgeArraySize(int rows, int columns){
    int firstColumn = 3 * (rows-1);
    int lastColumn = 2 * (rows - 1);
    int middleValues = 4 * (rows - 1 ) * (columns - 2);
    int lastRow = columns - 1;
    return firstColumn + lastColumn + middleValues + lastRow;
}

std::tuple<Edge**, int> getEdges(const cv::Mat &img, std::vector<Pixel *> &pixels){
    int rows = img.rows;
    int columns = img.cols;
    int edgeCount = 0;
    int edgeArraySize = getEdgeArraySize(rows, columns);
    Edge** edges = new Edge*[edgeArraySize];
    for(int row=0; row < rows; ++row){
        for(int column=0; column < columns; ++column) {
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
    std::cout << "Array Size: " << edgeArraySize << '\n';

    return {edges, edgeArraySize};
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
    static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };  // static used for efficiency, so we only calculate this value once
    // evenly distribute the random number across our range
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}

Edge** countSort(Edge** &edges, int edgeArraySize, int maxValue){
    Edge** sortedEdges = new Edge*[edgeArraySize];
    std::vector<int> countArray(maxValue + 1);

    for(int i=0; i<edgeArraySize;++i){
        countArray[edges[i]->weight] += 1;
    }

    for(int i=1; i<countArray.size();++i){
        countArray[i]+=countArray[i-1];
    }

    for(int j=edgeArraySize-1; j>=0; --j){
        sortedEdges[countArray[edges[j]->weight]-1] = edges[j];
        countArray[edges[j]->weight] -= 1;
    }

    return sortedEdges;
}