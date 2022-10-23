
OPENCV_FLAGS = `pkg-config --cflags --libs opencv`
CC = g++
CFLAGS = -Wall# -Werror
INCLUDE = -Isrc -Ilibs

all: pixel.o bayer.o PPG.o metrics main.o
	$(CC) pixel.o bayer.o PPG.o mse.o psnr.o main.o -o output `pkg-config --cflags --libs opencv`

main.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c main.cpp -std=c++17 `pkg-config --cflags --libs opencv`
	
pixel.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/pixel/pixel.cpp 

bayer.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/bayer_filter/bayer.cpp `pkg-config --cflags --libs opencv`

PPG.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/PPG/PPG.cpp `pkg-config --cflags --libs opencv`

metrics:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/metrics/mse.cpp `pkg-config --cflags --libs opencv`
	$(CC) $(CFLAGS) $(INCLUDE) -c src/metrics/psnr.cpp `pkg-config --cflags --libs opencv`

clean:
	rm *.o