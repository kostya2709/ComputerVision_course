#include <inttypes.h>
#include <opencv2/highgui.hpp>
#include <PPG/PPG.hpp>
#include <bayer_filter/bayer.hpp>
#include <iostream>


bool is_linear( int64_t known1, int64_t known2, int64_t known3)
{
    return ( known1 < known2 && known2 < known3) || ( known1 > known2 && known2 > known3);
}

int64_t hue_transit( int64_t known1, int64_t known2, int64_t known3, int64_t unknown1, int64_t unknown3)
{
    if ( is_linear( known1, known2, known3) )
    {
        return unknown1 + (unknown3 - unknown1) * (known2 - known1) / (known3 - known1);
    }
    return (unknown1 + unknown3) / 2 + (known2 * 2 - known1 - known3) / 4;
}

void calculate_at_red_blue( cv::Mat& src, uint32_t pad = 0)
{
    uint32_t height = src.rows;
    uint32_t width = src.cols;
    for ( uint32_t i = 0; i < height - 2 * pad; ++i)
    {
        for (uint32_t j = 0; j < width - 2 * pad; ++j)
        {
            uint32_t x = j + pad;
            uint32_t y = i + pad;

            colour known;
            colour unknown;

            colour cur_colour = get_bayer_colour( i, j);
            if ( cur_colour == colour::GREEN )
            {
                continue;
            } else if ( cur_colour == colour::BLUE )
            {
                known = colour::BLUE;
                unknown = colour::RED;
            } else
            {
                known = colour::RED;
                unknown = colour::BLUE;
            }

            SquareField<5> field( src, x, y);

            int64_t delta_NE = abs( (int64_t)field[9].get_colour( unknown) - field[17].get_colour( unknown)) +
                               abs( (int64_t)field[5].get_colour( known)   - field[13].get_colour( known)) +
                               abs( (int64_t)field[13].get_colour( known)  - field[21].get_colour( known)) +
                               abs( (int64_t)field[9].green  - field[13].green) +
                               abs( (int64_t)field[13].green - field[17].green);

            int64_t delta_NW = abs( (int64_t)field[7].get_colour( unknown) - field[19].get_colour( unknown)) +
                               abs( (int64_t)field[1].get_colour( known)   - field[13].get_colour( known)) +
                               abs( (int64_t)field[13].get_colour( known)  - field[25].get_colour( known)) +
                               abs( (int64_t)field[7].green  - field[13].green) +
                               abs( (int64_t)field[13].green - field[19].green);

            int64_t cur_val = 0;
            if ( delta_NE < delta_NW )
            {
                cur_val = hue_transit( field[9].green, field[13].green, field[17].green, field[9].get_colour( unknown), field[17].get_colour( unknown));
            } else
            {
                cur_val = hue_transit( field[7].green, field[13].green, field[19].green, field[7].get_colour( unknown), field[19].get_colour( unknown));
            }

            field.get_central().set_colour( unknown, saturate_pixel( cur_val));
        }
    }
}



void calculate_red_blue_at_green( cv::Mat& src, uint32_t pad = 0)
{
    uint32_t height = src.rows;
    uint32_t width = src.cols;
    for ( uint32_t i = 0; i < height - 2 * pad; ++i)
    {
        for (uint32_t j = 0; j < width - 2 * pad; ++j)
        {
            colour cur_colour = get_bayer_colour( i, j);
            if ( cur_colour != colour::GREEN )
            {
                continue;
            }

            uint32_t x = j + pad;
            uint32_t y = i + pad;

            SquareField<3> field( src, x, y);

            int64_t cur_blue;
            int64_t cur_red;

            /*
            * Green pixels in the even rows have blue neighbours horizontally and
            * red neighbours vertically. Green pixels in the odd rows - vice versa.
            */
            if ( y % 2 )
            {
                cur_blue = hue_transit( field[4].green, field[5].green, field[6].green, field[4].blue, field[6].blue);
                cur_red = hue_transit( field[2].green, field[5].green, field[8].green, field[2].red, field[8].red);
            } else
            {
                cur_red = hue_transit( field[4].green, field[5].green, field[6].green, field[4].red, field[6].red);
                cur_blue = hue_transit( field[2].green, field[5].green, field[8].green, field[2].blue, field[8].blue);
            }

            field.get_central().blue = saturate_pixel( cur_blue);
            field.get_central().red = saturate_pixel( cur_red);
        }
    }
}


void calculate_green( cv::Mat& src, uint32_t pad = 0)
{
    uint32_t height = src.rows;
    uint32_t width = src.cols;
    for ( uint32_t i = 0; i < height - 2 * pad; ++i)
    {
        for (uint32_t j = 0; j < width - 2 * pad; ++j)
        {
            colour cur_colour = get_bayer_colour( i, j);

            if ( cur_colour == colour::GREEN )
            {
                continue;
            }

            uint32_t x = j + pad;
            uint32_t y = i + pad;

            SquareField<5> field( src, x, y);

            int32_t delta_N = abs( field[13].get_colour( cur_colour) - field[3].get_colour( cur_colour))  * 2 + abs( field[8].green  - field[18].green);
            int32_t delta_E = abs( field[13].get_colour( cur_colour) - field[15].get_colour( cur_colour)) * 2 + abs( field[12].green - field[14].green);
            int32_t delta_W = abs( field[13].get_colour( cur_colour) - field[11].get_colour( cur_colour)) * 2 + abs( field[12].green - field[14].green);
            int32_t delta_S = abs( field[13].get_colour( cur_colour) - field[23].get_colour( cur_colour)) * 2 + abs( field[8].green  - field[18].green);

            int32_t cur_green;
            int32_t smallest = std::min( delta_N, std::min( delta_E, std::min( delta_W, delta_S)));
            if ( smallest == delta_N )
            {
                cur_green = (field[8].green * 3 + field[18].green + field[13].get_colour( cur_colour) - field[3].get_colour( cur_colour)) / 4;
            } else if ( smallest == delta_E)
            {
                cur_green = (field[14].green * 3 + field[12].green + field[13].get_colour( cur_colour) - field[15].get_colour( cur_colour)) / 4;
            } else if ( smallest == delta_W )
            {
                cur_green = (field[12].green * 3 + field[14].green + field[13].get_colour( cur_colour) - field[11].get_colour( cur_colour)) / 4;
            } else
            {
                cur_green = (field[18].green * 3 + field[8].green + field[13].get_colour( cur_colour) - field[23].get_colour( cur_colour)) / 4;
            }

            field.get_central().green = saturate_pixel( cur_green);
        }
    }
}

/*
* Expects init_image to be one-channel image
*/
cv::Mat make_PPG_demosaicing( cv::Mat init_image)
{
    cv::Mat padded_image;
    uint8_t pad = 0;
    cv::copyMakeBorder( init_image, padded_image, pad, pad, pad, pad, cv::BORDER_REFLECT);

    pad = 2;
    cv::Mat image = make_rgb_bayer( padded_image);
    calculate_green( image, pad);
    calculate_red_blue_at_green( image, pad);
    calculate_at_red_blue( image, pad);
    pad = 0;

    cv::Rect unpadded_area( pad, pad, init_image.cols, init_image.rows);
    cv::Mat cropped_image = image(unpadded_area);

    return cropped_image;
}
