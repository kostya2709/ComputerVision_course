#include <opencv2/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <pixel/pixel.hpp>
#include <median_filters/naive.hpp>
#include <median_filters/Huang_filter.hpp>
#include <median_filters/Perreault_filter.hpp>
#include <median_filters/OpenCV_filter.hpp>
#include <metrics/psnr.hpp>
#include <chrono>
#include <median_filters/naive.hpp>
#include <fstream>


int main( int argc, char** argv ) {

    cv::Mat init_image = cv::imread( "examples/median_filters/cat.bmp");

    std::ofstream output;
    output.open( "examples/median_filters/result.json");
    output << "{";

    const long double image_size = init_image.cols * init_image.rows / 1e6;

    const int filters_num = 4;
    cv::Mat (*MedianFilter[filters_num])(const cv::Mat&, uint32_t);
    MedianFilter[0] = naiveMedianFilter;
    MedianFilter[1] = HuangMedianFilter;
    MedianFilter[2] = PerreaultMedianFilter;
    MedianFilter[3] = OpenCVMedianFilter;

    std::vector<int> range = {5, 7, 9, 11, 15, 19, 23, 27, 35, 57, 71, 99};
    for ( int i = 0; i < range.size(); ++i )
    {
        output << '"' << range[i] << "\": [";
        for ( int foo_idx = 0; foo_idx < filters_num; ++foo_idx )
        {
            auto start = std::chrono::steady_clock::now();
            cv::Mat image = (*MedianFilter)( init_image, range[i]);
            auto end = std::chrono::steady_clock::now();
            
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / image_size;
            output << time;
            if ( foo_idx < filters_num - 1 )
            {
                output << ',';
            }
            auto name = std::string("examples/median_filters/generated/result_") + 
                        std::to_string(foo_idx) + "_" + std::to_string(range[i]) + ".bmp";
            cv::imwrite( name, image);
        }
        output << "]";
        if ( i <  range.size() - 1)
        {
            output << ',\n';
        }
    }

    output << "}\n";
    output.close();
    
    return 0;
}
