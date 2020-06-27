int getSingleIndex(int row, int col, int totalColumns);
int getEdgeArraySize(int rows, int columns);
std::vector<edge_pointer> setEdges(const std::vector<pixel_pointer> &pixels, std::string colorSpace, int rows, int columns);
cv::Mat addColorToSegmentation(component_struct_pointer componentStruct, int rows, int columns);
std::string getFileNameFromPath(const std::string &path);
void printParameters(const std::string &inputPath, const std::string &outputDir, const std::string &color,
                     float sigma, float k, int minimumComponentSize);
std::vector<pixel_pointer> constructImagePixels(const cv::Mat &img, int rows, int columns);
