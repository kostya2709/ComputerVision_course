#include <metrics/mse.hpp>
#include <iostream>

BigFloat mse( const cv::Mat& image1, const cv::Mat& image2)
{
    if ( image1.size != image2.size )
    {
        throw std::invalid_argument( "Images sizes do not match!");
    }

    if ( image1.channels() != image2.channels())
    {
        throw std::invalid_argument( "Images channel numbers do not match!");
    }

    if ( image1.channels() != 3 )
    {
        throw std::invalid_argument( "Only 3-channel images are supported!");
    }

    BigFloat result;
    uint64_t width  = image1.cols;
    uint64_t height = image1.rows;

    RGB_pixel* image1_data = (RGB_pixel*)image1.data;
    RGB_pixel* image2_data = (RGB_pixel*)image2.data;

    uint64_t cur_pos = 0;

    for ( uint64_t i = 0; i < width; ++i)
    {
        for (uint64_t j = 0; j < height; ++j, ++cur_pos)
        {
            const double luminance1 = calculate_luminance( image1_data[cur_pos]);
            const double luminance2 = calculate_luminance( image2_data[cur_pos]);
            
            result += pow(luminance1 - luminance2, 2);

            if ( cur_pos % 1000 == 0)
            {
                std::cout << cur_pos << "\n";
            }
        }
    }

    result /= (width * height);
    return result;
}