#pragma once

#include <opencv2/highgui.hpp>
#include <iostream>

template<typename FilterType>
cv::Mat traverse( const cv::Mat& src, uint32_t filter_size)
{
    cv::Mat padded_src;
    const int pad = filter_size / 2;
    cv::copyMakeBorder( src, padded_src, pad, pad, pad, pad, cv::BORDER_REPLICATE);
    uint32_t height = padded_src.rows;
    uint32_t width = padded_src.cols;
    cv::Mat result = padded_src.clone();

    if ( filter_size % 2 == 0 )
    {
        ++filter_size;
    }

    FilterType filter( filter_size, padded_src, result);

    for ( uint32_t i = 0; i < (width - (filter_size - 1)) * (height - (filter_size - 1)); ++i )
    {
        filter.step();
    }

    cv::Rect unpadded_area( pad, pad, src.cols, src.rows);
    cv::Mat cropped_image = result(unpadded_area);

    return cropped_image;
}
