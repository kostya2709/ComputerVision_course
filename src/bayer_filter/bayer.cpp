#include <inttypes.h>
#include <pixel/pixel.hpp>
#include <bayer_filter/bayer.hpp>

colour get_bayer_colour( uint32_t i, uint32_t j)
{
    if ( (i % 2 == 0) && (j % 2 == 0) )
    {
        return colour::RED;
    } else if ( (i % 2 == 1) && (j % 2 == 1) )
    {
        return colour::BLUE;
    }
    
    return colour::GREEN;
}

cv::Mat make_rgb_bayer( const cv::Mat& src)
{
    uint32_t height = src.rows;
    uint32_t width = src.cols;

    cv::Mat dst( height, width, CV_8UC3, cv::Scalar( 0, 0, 0));
    RGB_pixel* dst_data = (RGB_pixel*)dst.data;

    for ( uint32_t i = 0; i < height; ++i)
    {
        for ( uint32_t j = 0; j < width; ++j)
        {
            colour cur_colour = get_bayer_colour( i, j);
            dst_data[ width * i + j].set_colour( cur_colour, src.data[src.step * i + j * src.channels()]);
        }
    }
    return dst;
}