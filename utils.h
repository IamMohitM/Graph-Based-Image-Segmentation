int getSingleIndex( int row,int col,int totalColumns);
void quickSort(Edge** &edges, int startingIndex, int lastIndex, int &count);
int getRandomNumber(int min, int max);
int getEdgeArraySize(int rows, int columns);
void setEdges(const cv::Mat &img, std::vector<Pixel *> &pixels, std::vector<Edge*> &edges);
bool compareEdges(Edge* e1, Edge* e2);
cv::Mat addColorToSegmentation(ComponentStruct* componentStruct, int rows, int columns);
std::vector<std::string> split(const std::string& s, char seperator);