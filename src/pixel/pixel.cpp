#include "pixel.hpp"

int32_t RGB_pixel::get_colour( colour colour) const
{
    return *((uint8_t*)this + colour);
}

void RGB_pixel::set_colour( colour colour, uint8_t val)
{
    *((uint8_t*)this + colour) = val;
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
