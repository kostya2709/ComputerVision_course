#include <median_filters/naive.hpp>
#include <median_filters/base_filter.hpp>
#include <median_filters/image_traverse.hpp>
#include <vector>

class NaiveFilter : public BaseFilter
{
public:
    NaiveFilter( const uint32_t filter_size, cv::Mat& src) : BaseFilter( filter_size, src) {}
    
    void step()
    {
        if ( _cur_y == (kImageHeight - kFilterSize / 2) )
        {
            return;
        }

        SquareField field( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize);
        const uint32_t channel_num = 3;
        std::vector<uint8_t> pixels[channel_num];
        const uint64_t area = kFilterSize * kFilterSize;

        for ( uint32_t i = 0; i < area; ++i )
        {
            for ( uint32_t channel = 0;  channel < channel_num; ++channel )
            {
                pixels[channel].push_back( field[i + 1].get_colour( (colour)channel));
            }
        }

        for ( uint32_t channel = 0;  channel < channel_num; ++channel )
        {
            std::sort( pixels[channel].begin(), pixels[channel].end());
        }

        RGB_pixel mean_pixel;
        for ( uint32_t channel = 0;  channel < channel_num; ++channel )
        {
            mean_pixel.set_colour( (colour)channel, pixels[channel][area / 2]);
        }
        field.get_central() = mean_pixel;

        ++_cur_x;
        if ( _cur_x == (kImageWidth - kFilterSize / 2) )
        {
            ++_cur_y;
            _cur_x = kFilterSize / 2;
        }
    }
};

cv::Mat naiveMedianFilter( const cv::Mat& init_image, const uint32_t filter_size)
{
    cv::Mat result( init_image);
    traverse<NaiveFilter>( result, filter_size);
    return result;
}
