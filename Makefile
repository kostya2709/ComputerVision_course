
OPENCV_FLAGS = `pkg-config --cflags --libs opencv`
CC = g++
CFLAGS = -Wall# -Werror
INCLUDE = -Isrc -Ilibs

all: pixel.o bayer.o PPG.o metrics median_filters main.o
	$(CC) pixel.o bayer.o PPG.o Huang_filter.o Perreault_filter.o OpenCV_filter.o naive.o mse.o psnr.o main.o -o output `pkg-config --cflags --libs opencv`

main.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c main.cpp -std=c++17 `pkg-config --cflags --libs opencv`
	
pixel.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/pixel/pixel.cpp -std=c++17 `pkg-config --cflags --libs opencv`

bayer.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/bayer_filter/bayer.cpp `pkg-config --cflags --libs opencv`

PPG.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/PPG/PPG.cpp `pkg-config --cflags --libs opencv`

median_filters:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/median_filters/naive.cpp `pkg-config --cflags --libs opencv`
	$(CC) $(CFLAGS) $(INCLUDE) -c src/median_filters/Huang_filter.cpp `pkg-config --cflags --libs opencv`
	$(CC) $(CFLAGS) $(INCLUDE) -c src/median_filters/Perreault_filter.cpp `pkg-config --cflags --libs opencv`
	$(CC) $(CFLAGS) $(INCLUDE) -c src/median_filters/OpenCV_filter.cpp `pkg-config --cflags --libs opencv`

metrics:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/metrics/mse.cpp `pkg-config --cflags --libs opencv`
	$(CC) $(CFLAGS) $(INCLUDE) -c src/metrics/psnr.cpp `pkg-config --cflags --libs opencv`

clean:
	rm *.o