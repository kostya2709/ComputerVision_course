#pragma once

#include <inttypes.h>
#include <opencv2/highgui.hpp>

enum colour
{
  BLUE,
  GREEN,
  RED
};

struct RGB_pixel
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;

    int32_t get_colour( colour colour) const;
    void    set_colour( colour colour, uint8_t val);

    uint8_t& operator[]( uint32_t idx);
    uint8_t operator[]( uint32_t idx) const;
};

uint8_t saturate_pixel( int64_t val);
double calculate_luminance( RGB_pixel pixel);


class SquareField
{
public:

    SquareField() = delete;
    SquareField( const SquareField&) = delete;
    SquareField( const cv::Mat& src, uint32_t x, uint32_t y, const uint32_t size);
    SquareField( RGB_pixel* data_ptr, uint32_t image_width, 
                 uint32_t x, uint32_t y, const uint32_t size);

    RGB_pixel operator[] ( uint32_t index) const;
    RGB_pixel& operator[] ( uint32_t index);
    RGB_pixel get_central() const;
    RGB_pixel& get_central();
    uint32_t area() const;
    uint32_t size() const;

private:
    const uint32_t kSize;
    RGB_pixel* const kSrcBufferPtr;
    uint64_t src_width;
};
