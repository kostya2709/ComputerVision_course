#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <pixel/pixel.hpp>
#include <FHT/FastHoughTransform.hpp>
#include <rotate/nearest_neighbour.hpp>
#include <chrono>
#include <median_filters/naive.hpp>


int main( int argc, char** argv ) {

    for ( int i = 1; i < 11; ++i )
    {
        cv::Mat init_image = cv::imread( "examples/FHT/original/" + std::to_string(i) + ".jpg");

        cv::Mat transformed = FastHoghTransform( init_image, nearestNeighbourRotate, i);
        // cv::imshow("Display window", transformed);
        // int k = cv::waitKey(0); // Wait for a keystroke in the window

        cv::imwrite( "examples/FHT/transformed/" + std::to_string(i) + "_res.jpg", transformed);
    }

    return 0;
}
