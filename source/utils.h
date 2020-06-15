int getSingleIndex(const int row, const int col, const int totalColumns);
int getEdgeArraySize(const int rows,const int columns);
std::vector<Edge*>& setEdges(const cv::Mat &img, const std::vector<Pixel *> &pixels, std::vector<Edge*> &edges);
std::vector<Edge*>& setRGBEdges(const cv::Mat &img, const std::vector<Pixel *> &pixels, std::vector<Edge*> &edges);
bool compareEdges(const Edge* e1,const Edge* e2);
cv::Mat addColorToSegmentation(const ComponentStruct* componentStruct, int rows, int columns);
std::string getFileNameFromPath(const std::string &path);
std::vector<Pixel *>& constructImageGraph(const cv::Mat& img, std::vector<Pixel *> &pixels, int rows, int columns);
std::vector<Pixel *>& constructRGBImageGraph(const cv::Mat& img, std::vector<Pixel *> &pixels, int rows, int columns);
void printParameters(const std::string &inputPath, const std::string &outputDir, const std::string &color,
                     const float sigma, const float k, const int minimumComponenetSize);