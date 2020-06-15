cmake_minimum_required(VERSION 3.16)
project(GraphBasedImageSegmentation)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
add_executable(ImageSegment main.cpp DisjointForest.cpp utils.cpp segmentation.cpp)

message(STATUS " include_path: ${OpenCV_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(ImageSegment PUBLIC ${OpenCV_LIBS})
message(STATUS " Libraries: ${OpenCV_LIBS}")