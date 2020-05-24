int getSingleIndex( int row,int col,int totalColumns);
bool checkValidityOfGraph(int rows, int cols, const cv::Mat &img, const std::vector<Pixel *> &pixels);
std::tuple<Edge**, int> getEdges(const cv::Mat &img, std::vector<Pixel *> &pixels);
void quickSort(Edge** &edges, int startingIndex, int lastIndex);
void quickSort(Edge** &edges, int startingIndex, int lastIndex, int &count);
int getRandomNumber(int min, int max);