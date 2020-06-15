# Graph-Based-Image-Segmentation
This project is an attempt to implement the research paper: [Efficient Graph Based Image Segmentation](http://people.cs.uchicago.edu/~pff/papers/seg-ijcv.pdf).
Entire bottom-up implementation done by the author (post-processing and rgb-color difference ideas taken from [here](http://cs.brown.edu/people/pfelzens/segment/)).
 
### Requirements
1. C++ standard 17
2. OpenCV

### Building and Execution
Run the commands in your project directory
```
mkdir build
cd build
cmake ../source
cmake --build .
```

Execute the program while in build directory
```
.\ImageSegment inputImage outputDir colorSpace k(float) sigma(float) minSize(int)
```
### Results
| Original Image                                      | Results                                                                                                             |
|--------------                                       |-------- |
|![Image Baseball](images/baseball.png)             | sigma = 1.5 k = 1000 min = 100 grayscale ![Results Baseball](Results/baseball-gray-k1000-1.500000-min100.jpg)        |
|||
|![Image Chateau-de-Chenonceau](images/chateau-de-chenonceau.jpg)| sigma = 0.8 k = 1000 min = 100 grayscale![Results Chateau-de-Chenonceau](Results/chateau-de-chenonceau-gray-k1000-0.800000-min50.jpg)     |
|||
|![Image Paris](images/paris.jpg)                | sigma = 1.0 k = 1000 min = 100 RGB![Results Paris](Results/paris-rgb-k1000-1.000000-min100.jpg)                         |
|||
|![Image Eiffel Tower](images/eiffel-tower.jpg)     | sigma = 1.0 k = 800 min = 100 RGB![Results Eiffel Tower](Results/eiffel-tower-rgb-k800-1.000000-min100.jpg)                |
|||
|![Image Versailles Gardens](images/versailles-gardens.jpg)|sigma = 1.0 k = 750 min = 100 Grayscale![Results Versailles Gardens](Results/versailles-gardens-gray-k750-1.000000-min100.jpg)              |


### TODO
- [ ] Fix Relative Paths<br>
- [ ] Add makefile installation
- [ ] Remove raw pointers and use unique_ptr instead
- [ ] Validate pointer parameters to functions (prefer passing by reference instead) 

### License
[GNU-GPL](https://choosealicense.com/licenses/gpl-3.0/)

### Note
The files in Results folder are names in format : *filename-[gray|rgb]-k{value}-{sigmaValue}-min{minimumComponents}.jpg*


