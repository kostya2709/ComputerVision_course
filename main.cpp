#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <pixel/pixel.hpp>
#include <PPG/PPG.hpp>
#include <metrics/psnr.hpp>
#include <chrono>


int main( int argc, char** argv ) {

    cv::Mat init_image = cv::imread( "RGB_CFA.bmp", cv::IMREAD_GRAYSCALE);
    cv::Mat orig_image = cv::imread( "Original.bmp");

    std::cout << "Orig: " << orig_image.cols << " " << orig_image.rows << "\n";
    std::cout << "CFA: " << init_image.cols << " " << init_image.rows << "\n";


    auto start = std::chrono::steady_clock::now();
    cv::Mat image = make_PPG_demosaicing( init_image);
    auto end = std::chrono::steady_clock::now();

    std::cout << image.cols << " " << image.rows << "\n";


    double psnr_val = psnr( orig_image, image);
    std::cout << "PSNR " << psnr_val << '\n';


    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " sec";


    cv::imwrite( "result.bmp", image);
    
    cv::waitKey(0);
    return 0;
}
