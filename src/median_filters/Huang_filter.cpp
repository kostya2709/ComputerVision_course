#include <median_filters/Huang_filter.hpp>
#include <median_filters/base_filter.hpp>
#include <median_filters/image_traverse.hpp>
#include <vector>

class Storage
{
public:

    Storage( const SquareField& field)
    {
        init( field);
    }

    void init( const SquareField& field)
    {
        const uint32_t area = field.area();
        std::vector<uint8_t> pixels[CHANNEL_NUM];
        for ( uint8_t channel = 0; channel < CHANNEL_NUM; ++channel )
        {
            _less_median[channel] = 0;
            for ( uint32_t i = 0; i < area; ++i )
            {
                uint32_t cur_val = field[i + 1][channel];
                pixels[channel].push_back( cur_val);
                ++_data[channel][cur_val];
            }

            std::sort( pixels[channel].begin(), pixels[channel].end());

            const uint8_t median_val = pixels[channel][area / 2];
            _cur_median[channel] = median_val;
            for ( uint32_t i = 0; i < area; ++i )
            {
                uint8_t cur_val = pixels[channel][i];
                if ( cur_val < median_val )
                {
                    ++_less_median[channel];
                } else
                {
                    break;
                }
            }

            for ( uint32_t i = 0; i < area; ++i )
            {
                std::cout << (int)pixels[channel][i] << ' ';
            }
            std::cout << "\nLESS MEDIAN INIT " << _less_median[channel] << '\n';
        }
    }

    RGB_pixel getMedian()
    {
        return _cur_median;
    }

    /*
    * Erase the left column of the field
    */
    void update( const SquareField& field_old, const SquareField& field_new)
    {
        const uint32_t field_size = field_old.size();
        uint32_t half_area = field_new.area() / 2;
        for ( uint8_t channel = 0; channel < CHANNEL_NUM; ++channel )
        {
            const uint8_t median_val = _cur_median[channel];
            for ( uint32_t k = 0; k < field_size; ++k )
            {
                const uint8_t delete_val = field_old[k * field_size + 1][channel];
                if ( delete_val < median_val )
                {
                    std::cout << "delete val " << (int)delete_val << '\n';
                    std::cout << "median val " << (int)median_val << '\n';
                    for ( uint32_t i = 0; i < field_size * field_size; ++i )
                    {
                        std::cout << _data[channel][i] << ' ';
                    }
                    std::cout << '\n';
                    --_less_median[channel];
                    assert( _less_median[channel] >= 0 );
                }
                --_data[channel][delete_val];
                assert( _data[channel][delete_val] >= 0 );

                const uint8_t insert_val = field_new[(k + 1) * field_size][channel];
                if ( insert_val < median_val )
                {
                    ++_less_median[channel];
                }
                ++_data[channel][insert_val];
            }

            // Update the median
            while ( _less_median[channel] > half_area )
            {
                --_cur_median[channel];
                assert( _cur_median[channel] >= 0 );
                _less_median[channel] -= _data[channel][_cur_median[channel]];
                assert( _less_median[channel] >= 0 );
            }

            while ( _less_median[channel] + _data[channel][_cur_median[channel]] <= half_area )
            {
                _less_median[channel] += _data[channel][_cur_median[channel]];
                ++_cur_median[channel];
            }
        }
    }

    void clear()
    {
        memset( _data, 0, CHANNEL_NUM * VALUE_RANGE);
    }
    
private:

    enum
    {
        CHANNEL_NUM = 3,
        VALUE_RANGE = 256
    };

    int32_t _data[CHANNEL_NUM][VALUE_RANGE] = {};
    RGB_pixel _cur_median;
    int32_t _less_median[CHANNEL_NUM] = {};
};

class HuangFilter : public BaseFilter
{
public:
    HuangFilter( const uint32_t filter_size, cv::Mat& src) : 
                 BaseFilter( filter_size, src),
                 _storage( SquareField ( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize)){}
    
    void step()
    {
        if ( _cur_y == (kImageHeight - kFilterSize / 2) )
        {
            return;
        }

        SquareField field_old( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize);

        ++_cur_x;
        if ( _cur_x == (kImageWidth - kFilterSize / 2) )
        {
            ++_cur_y;

            if ( _cur_y == (kImageHeight - kFilterSize / 2) )
            {
                return;
            }

            _cur_x = kFilterSize / 2;
            SquareField field_new( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize);
            _storage.clear();
            _storage.init( field_new);
            // field_new.get_central() = _storage.getMedian();
            return;
        }

        SquareField field_new( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize);
        _storage.update( field_old, field_new);
        // field_new.get_central() = _storage.getMedian();
    }
private:
    Storage _storage;
};

cv::Mat HuangMedianFilter( const cv::Mat& init_image, const uint32_t filter_size)
{
    cv::Mat result( init_image);
    traverse<HuangFilter>( result, filter_size);
    return result;
}
