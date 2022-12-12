
OPENCV_FLAGS = `pkg-config --cflags --libs opencv`
CC = g++
CFLAGS = #-Wall# -Werror
INCLUDE = -Isrc -Ilibs

all: FHT rotation main.o
	$(CC) bilinear.o nearest_neighbour.o FastHoughTransform.o main.o -o output `pkg-config --cflags --libs opencv`

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

FHT:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/FHT/FastHoughTransform.cpp `pkg-config --cflags --libs opencv`

rotation:
	$(CC) $(CFLAGS) $(INCLUDE) -c src/rotate/nearest_neighbour.cpp `pkg-config --cflags --libs opencv`
	$(CC) $(CFLAGS) $(INCLUDE) -c src/rotate/bilinear.cpp `pkg-config --cflags --libs opencv`

clean:
	rm *.o