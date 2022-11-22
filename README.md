# Demosaicing

PPG algorithm was used. 

image size: 4176 * 2073 = 8'656'848 pixels

Speed: 128.853 millisec / Mpixel
Speed is rather high, because everything is implemented in C++ and rather optimally.

PNSR = 19.371

Something went wrong, because the result image is too green. Also I didn't process the edges perfectly.


![origin](https://github.com/kostya2709/Computer_Vision_course/blob/master/Original.bmp)
![result](https://github.com/kostya2709/Computer_Vision_course/blob/master/result.bmp)

To implement the algorithm, the following article was used:
![article_demosaicing](https://web.archive.org/web/20160923211135/https://sites.google.com/site/chklin/demosaic/)

# Median Filters

Median Filters find the mean value of pixels in a sliding window and substitute a central pixel with this median value. They are often used to subdue salt-and-pepper noise. 
In this work, three algorithms of median filters were implemented, their performance were compared, conclusions were done.

## Example

Let's take an image of a cat, add some salt-and-pepper noise and try to remove or at least to smooth it.

