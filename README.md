# Demosaicing

PPG algorithm was used. 

image size: 4176 * 2073 = 8'656'848 pixels

Speed: 188.42 millisec / Mpixel
Speed is rather high, because everything is implemented in C++ and rather optimally.

PNSR = 16.149

Something went wrong, because the result image is too green. Also I didn't process the edges perfectly.

I also didn't find a decent library for long floating point arithmetic for C++. So I made a class-wrapper,
which stores a value like a string and calculates everything via Python. But this method requires writing to file
and therefore is really slow. So I implemented PSNR on Python.

![origin](https://github.com/kostya2709/Computer_Vision_course/blob/master/Original.bmp)
![result](https://github.com/kostya2709/Computer_Vision_course/blob/master/result.bmp)

To implement the algorithm, the following article was used:
![article_demosaicing](https://web.archive.org/web/20160923211135/https://sites.google.com/site/chklin/demosaic/)

# 