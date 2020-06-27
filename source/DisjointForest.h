//
// Created by mo on 21/05/20.
//
#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <opencv2/opencv.hpp>

struct Component;
struct ComponentStruct;
struct Pixel;
struct Edge;

using component_pointer = std::shared_ptr<Component>;
using component_struct_pointer = std::shared_ptr<ComponentStruct>;
using pixel_pointer = std::shared_ptr<Pixel>;
using edge_pointer = std::shared_ptr<Edge>;

struct Pixel{
    component_pointer parentTree;
    pixel_pointer parent;
    int intensity;
    int bValue;
    int gValue;
    int rValue;
    int row;
    int column;
};

struct Component{
    component_struct_pointer parentComponentStruct;
    std::vector<pixel_pointer> pixels;
    int rank = 0;
    pixel_pointer representative;
    double MSTMaxEdge = 0;
};

struct Edge{
    double weight;
    pixel_pointer n1;
    pixel_pointer n2;
};

struct ComponentStruct{
    component_struct_pointer previousComponentStruct=nullptr;
    component_pointer component{};
    component_struct_pointer nextComponentStruct= nullptr;
};

edge_pointer createEdge(const pixel_pointer& pixel1, const pixel_pointer& pixel2, const std::function<double(pixel_pointer , pixel_pointer)>& edgeDifferenceFunction);
double rgbPixelDifference(const pixel_pointer& pixel1, const pixel_pointer& pixel2);
double grayPixelDifference(const pixel_pointer& pixel1, const pixel_pointer& pixel2);
void mergeComponents(const component_pointer& x, const component_pointer& y, double MSTMaxEdgeValue);
component_pointer makeComponent(int row, int column, const cv::Vec3b& pixelValues);