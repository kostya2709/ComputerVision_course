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

        cv::Mat transformed = FastHoghTransform( init_image, bilinearRotate, i);
        // cv::imshow("Display window", transformed);
        // int k = cv::waitKey(0); // Wait for a keystroke in the window

        cv::imwrite( "examples/FHT/transformed/" + std::to_string(i) + "_res.jpg", transformed);
    }

    // std::cout << "{" << std::flush;
    // int test_num = 14;
    // for ( int i = 0; i < test_num; ++i )
    // {
    //     std::cout << "\"" << i << "\" : ";
    //     double size = pow( 2, i);
    //     cv::Mat test_img( size, size, CV_32SC1);
    //     auto start = std::chrono::steady_clock::now();
    //     auto res = FastHoughAlgorithm( test_img);
    //     auto end = std::chrono::steady_clock::now();

    //     auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    //     std::cout << "\"" << time << "\" " << std::flush;

    //     if ( i < test_num - 1 )
    //     {
    //         std::cout << ',' << std::flush;
    //     }
    // }

    // std::cout << "}\n" << std::flush;


    // cv::Mat init_image = cv::imread( "examples/FHT/original/1.jpg");
    // cv::Mat rotated_n = nearestNeighbourRotate( init_image, M_PI_4);
    // cv::Mat rotated_b = bilinearRotate( init_image, M_PI_4);
    // cv::imwrite( "examples/FHT/transformed/1_nearest.jpg", rotated_n);
    // cv::imwrite( "examples/FHT/transformed/1_bilinear.jpg", rotated_b);

    return 0;
}
