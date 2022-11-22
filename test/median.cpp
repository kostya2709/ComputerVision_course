#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <pixel/pixel.hpp>
#include <median_filters/naive.hpp>
#include <median_filters/Huang_filter.hpp>
#include <median_filters/Perreault_filter.hpp>
#include <median_filters/OpenCV_filter.hpp>
#include <chrono>
#include <median_filters/naive.hpp>
#include <fstream>


int test_median( int argc, char** argv ) {

    cv::Mat init_image = cv::imread( "examples/median_filters/noisy_cat/noisy_cat.bmp");
    std::array<int, 5> filter_size = {3, 5, 7, 11, 27};

    for ( auto size : filter_size )
    {
        cv::Mat image = HuangMedianFilter( init_image, size);
        auto name = std::string( "examples/median_filters/noisy_cat/noisy_cat_") + std::to_string( size) + ".bmp";
        cv::imwrite( name, image);
    }
    return 0;
}