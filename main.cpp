#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <pixel/pixel.hpp>
#include <PPG/PPG.hpp>
#include <metrics/psnr.hpp>
#include <chrono>


int main( int argc, char** argv ) {

    cv::Mat init_image = cv::imread( "CFA.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat orig_image = cv::imread( "Original.bmp");


    auto start = std::chrono::steady_clock::now();
    cv::Mat image = make_PPG_demosaicing( init_image);
    auto end = std::chrono::steady_clock::now();


    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " millisec";


    // cv::imshow("Display window", image);
    // int k = cv::waitKey(0); // Wait for a keystroke in the window

    cv::imwrite( "result.bmp", image);
    
    return 0;
}
