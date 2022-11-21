#pragma once

#include <opencv2/highgui.hpp>
#include <iostream>

template<typename FilterType>
void traverse( const cv::Mat& src, cv::Mat& result, uint32_t filter_size)
{
    uint32_t height = src.rows;
    uint32_t width = src.cols;

    if ( filter_size % 2 == 0 )
    {
        ++filter_size;
    }

    FilterType filter( filter_size, src, result);

    for ( uint32_t i = 0; i < (width - (filter_size - 1)) * (height - (filter_size - 1)); ++i )
    {
        std::cout << "Step " << i << '\n';
        filter.step();
    }
}
