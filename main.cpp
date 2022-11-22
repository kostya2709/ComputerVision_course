#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <pixel/pixel.hpp>
#include <median_filters/naive.hpp>
#include <median_filters/Huang_filter.hpp>
#include <median_filters/Perreault_filter.hpp>
#include <metrics/psnr.hpp>
#include <chrono>
#include <median_filters/naive.hpp>


int main( int argc, char** argv ) {

    cv::Mat init_image = cv::imread( "cat.bmp");

    uint32_t filter_size = 5;
    auto start = std::chrono::steady_clock::now();
    cv::Mat image = PerreaultMedianFilter( init_image, filter_size);
    auto end = std::chrono::steady_clock::now();


    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " millisec";


    cv::imshow("Display window", image);
    int k = cv::waitKey(0); // Wait for a keystroke in the window

    // cv::imwrite( "result_filter.bmp", image);
    
    return 0;
}
