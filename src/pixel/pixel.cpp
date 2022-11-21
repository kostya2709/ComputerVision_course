#include "pixel.hpp"

int32_t RGB_pixel::get_colour( colour colour) const
{
    return *((uint8_t*)this + colour);
}

void RGB_pixel::set_colour( colour colour, uint8_t val)
{
    *((uint8_t*)this + colour) = val;
}

uint8_t& RGB_pixel::operator[]( uint32_t idx)
{
    assert( idx < 3 );
    return *((uint8_t*)this + idx);
}

uint8_t RGB_pixel::operator[]( uint32_t idx) const
{
    assert( idx < 3 );
    return *((uint8_t*)this + idx);
}

uint8_t saturate_pixel( int64_t val)
{
    if ( val > UINT8_MAX)
    {
        return UINT8_MAX;
    } else if ( val < 0 )
    {
        return 0;
    }
    return val;
}

double calculate_luminance( RGB_pixel pixel)
{
    const double RED_COEFF = 0.299;
    const double GREEN_COEFF = 0.587;
    const double BLUE_COEFF = 0.114;

    return RED_COEFF * pixel.red + GREEN_COEFF * pixel.green + BLUE_COEFF * pixel.blue;
}

SquareField::SquareField( const cv::Mat& src, uint32_t x, uint32_t y, const uint32_t size) : 
                kSize( size),
                kSrcBufferPtr( &((RGB_pixel*)src.data)[src.cols * (y - size / 2) + (x - size / 2)]),
                src_width( src.cols) {}

SquareField::SquareField( RGB_pixel* data_ptr, uint32_t image_width, 
                uint32_t x, uint32_t y, const uint32_t size) :
                kSize( size),
                kSrcBufferPtr( &((RGB_pixel*)data_ptr)[image_width * (y - size / 2) + (x - size / 2)]),
                src_width( image_width) {}

RGB_pixel SquareField::operator[] ( uint32_t index) const
{
    index -= 1;
    assert( index < area() );
    uint64_t line_num = index / kSize;
    uint64_t column_num = index % kSize;
    return kSrcBufferPtr[line_num * src_width + column_num];
}

RGB_pixel& SquareField::operator[] ( uint32_t index)
{
    index -= 1;
    assert( index < area() );
    uint64_t line_num = index / kSize;
    uint64_t column_num = index % kSize;
    return kSrcBufferPtr[line_num * src_width + column_num];
}

RGB_pixel& SquareField::get_central()
{
    return this->operator[]( (kSize / 2) * (kSize + 1) + 1);
}

RGB_pixel SquareField::get_central() const
{
    return this->operator[]( (kSize / 2) * (kSize + 1) + 1);
}

uint32_t SquareField::area() const
{
    return kSize * kSize;
}

uint32_t SquareField::size() const
{
    return kSize;
}
