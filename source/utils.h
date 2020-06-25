int getSingleIndex(const int row, const int col, const int totalColumns);
int getEdgeArraySize(const int rows,const int columns);
std::vector<Edge *> setEdges(const std::vector<Pixel *> &pixels, std::string colorSpace, int rows, int columns);
cv::Mat addColorToSegmentation(const ComponentStruct* componentStruct, int rows, int columns);
std::string getFileNameFromPath(const std::string &path);
void printParameters(const std::string &inputPath, const std::string &outputDir, const std::string &color,
                     const float sigma, const float k, const int minimumComponentSize);
std::vector<Pixel *> constructImagePixels(const cv::Mat &img, int rows, int columns);
