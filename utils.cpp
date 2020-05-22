#include <opencv2/opencv.hpp>
#include <vector>
#include <tuple>
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