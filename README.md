# Demosaicing

PPG algorithm was used. 

image size: 4176 * 2073 = 8'656'848 pixels

Speed: 128.853 millisec / Mpixel
Speed is rather high, because everything is implemented in C++ and rather optimally.

PNSR = 19.371

Something went wrong, because the result image is too green. Also I didn't process the edges perfectly.


![origin](https://github.com/kostya2709/Computer_Vision_course/blob/master/Original.bmp)
![result](https://github.com/kostya2709/Computer_Vision_course/blob/master/result.bmp)

To implement the algorithm, the following
[article](https://web.archive.org/web/20160923211135/https://sites.google.com/site/chklin/demosaic/)
was used.

# Median Filters

Median Filters find the mean value of pixels in a sliding window and substitute a central pixel with this median value. They are often used to subdue salt-and-pepper noise. 
In this work, three algorithms of median filters were implemented, their performance were compared, conclusions were done.

## Example

Let's take an image of a cat, add some salt-and-pepper noise and try to remove or at least to smooth it.

![origin](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/cat.bmp)
![noisy_cat](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/noisy_cat/noisy_cat.bmp)

We can eliminate noise with median filters. For instance, we can use Huang filter.
Changing the filter size, blurring also changes.

![R = 3](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/noisy_cat/noisy_cat_3.bmp)
![R = 5](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/noisy_cat/noisy_cat_5.bmp)
![R = 7](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/noisy_cat/noisy_cat_7.bmp)
![R = 11](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/noisy_cat/noisy_cat_11.bmp)
![R = 27](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/noisy_cat/noisy_cat_27.bmp)

## Median filter algorithms

There are three observed median filter algorithms: naive sorting, [Huang algorithm](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1163188), and [Perreault algorithm](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=4287006). In the [notebook](https://github.com/kostya2709/ComputerVision_course/blob/master/examples/median_filters/result.ipynb) below, time and memory comparison in demonstrated.