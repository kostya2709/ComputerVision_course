#pragma once

#include <inttypes.h>

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
};

uint8_t saturate_pixel( int64_t val);
double calculate_luminance( RGB_pixel pixel);
