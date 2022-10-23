#pragma once

#include <opencv2/highgui.hpp>
#include <pixel/pixel.hpp>

cv::Mat make_PPG_demosaicing( cv::Mat init_image);

template <uint8_t size>
class SquareField
{
public:

    SquareField() = delete;
    SquareField( const SquareField&) = delete;

    SquareField( const cv::Mat& src, uint32_t x, uint32_t y) : src_width( src.cols)
    {
        RGB_pixel* data_ptr = (RGB_pixel*)src.data; 
        src_buffer_ptr = &data_ptr[src.cols * (y - 2) + (x - 2)];
    }

    RGB_pixel& operator[] ( uint32_t index) const
    {
        index -= 1;
        uint64_t line_num = index / size;
        uint64_t column_num = index % size;
        return src_buffer_ptr[line_num * src_width + column_num];
    }

    RGB_pixel& get_central() const
    {
        return this->operator[]( (size / 2) * (size + 1) + 1);
    }

private:
    RGB_pixel* src_buffer_ptr;
    uint64_t src_width;
};
