#include <median_filters/Perreault_filter.hpp>
#include <median_filters/base_filter.hpp>
#include <median_filters/image_traverse.hpp>
#include <vector>

class PerreaultFilter : public BaseFilter
{
    class Storage
    {
    public:

        Storage( const SquareField& field, const cv::Mat& src) : _data_ptr( (RGB_pixel*)src.data),
                                                                 _cols_num( src.cols)
        {
            const uint32_t filter_size = field.size();
            for ( int channel = 0; channel < CHANNEL_NUM; ++channel )
            {
                for ( uint32_t cur_col = 0; cur_col < _cols_num; ++cur_col )
                {
                    _cols[channel].push_back( std::vector<uint32_t>( VALUE_RANGE, 0));
                    for ( uint32_t i = 0; i < filter_size; ++i )
                    {
                        const uint8_t cur_val = _data_ptr[_cols_num * i + cur_col][channel];
                        ++_cols[channel][cur_col][cur_val];
                    }
                }
            }
            init( field, filter_size / 2);
        }

        void init( const SquareField& field, const uint32_t cur_y)
        {
            const uint32_t area = field.area();
            const uint32_t filter_size = field.size();
            std::vector<uint8_t> pixels[CHANNEL_NUM];
            for ( uint8_t channel = 0; channel < CHANNEL_NUM; ++channel )
            {
                if ( cur_y != filter_size / 2 )
                {
                    for ( uint32_t cur_col = 0; cur_col < filter_size; ++cur_col )
                    {
                        const uint8_t deleting = _data_ptr[_cols_num * (cur_y - filter_size / 2 - 1) + cur_col][channel];
                        const uint8_t inserting = _data_ptr[_cols_num * (cur_y + filter_size / 2) + cur_col][channel];
                        --_cols[channel][cur_col][deleting];
                        ++_cols[channel][cur_col][inserting];
                    }
                }

                _less_median[channel] = 0;
                for ( uint32_t i = 0; i < area; ++i )
                {
                    uint8_t cur_val = field[i + 1][channel];
                    pixels[channel].push_back( cur_val);
                    ++_data[channel][cur_val];
                }

                std::sort( pixels[channel].begin(), pixels[channel].end());

                const uint8_t median_val = pixels[channel][area / 2];
                _cur_median[channel] = median_val;
                for ( uint32_t i = 0; i < median_val; ++i )
                {
                    _less_median[channel] += _data[channel][i];
                }
            }
        }

        RGB_pixel getMedian()
        {
            return _cur_median;
        }

        void update( const uint32_t filter_size, const uint32_t cur_x, const uint32_t cur_y)
        {
            uint32_t half_area = filter_size * filter_size / 2;
            for ( uint8_t channel = 0; channel < CHANNEL_NUM; ++channel )
            {
                if ( cur_y != filter_size / 2 )
                {
                    const uint8_t deleting = _data_ptr[_cols_num * (cur_y - filter_size / 2 - 1) + cur_x + filter_size / 2][channel];
                    const uint8_t inserting = _data_ptr[_cols_num * (cur_y + filter_size / 2) + cur_x + filter_size / 2][channel];
                    --_cols[channel][cur_x + filter_size / 2][deleting];
                    ++_cols[channel][cur_x + filter_size / 2][inserting];
                }

                const uint8_t median_val = _cur_median[channel];
                for ( uint32_t clr = 0; clr < VALUE_RANGE; ++clr )
                {
                    int32_t delete_num = _cols[channel][cur_x - filter_size / 2 - 1][clr];
                    int32_t insert_num = _cols[channel][cur_x + filter_size / 2][clr];
                    int32_t delta = insert_num - delete_num;
                    _data[channel][clr] += delta;
                    if ( clr < median_val )
                    {
                        _less_median[channel] += delta;
                    }
                }

                // Update the median
                while ( _less_median[channel] > half_area )
                {
                    --_cur_median[channel];
                    _less_median[channel] -= _data[channel][_cur_median[channel]];
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
            memset( _data, 0, CHANNEL_NUM * VALUE_RANGE * sizeof(_data[0][0]));
        }
        
    private:

        enum
        {
            CHANNEL_NUM = 3,
            VALUE_RANGE = 256
        };

        uint32_t _data[CHANNEL_NUM][VALUE_RANGE] = {};
        RGB_pixel _cur_median;
        uint32_t _less_median[CHANNEL_NUM] = {};
        std::vector<std::vector<uint32_t>> _cols[CHANNEL_NUM];
        RGB_pixel* _data_ptr;
        int32_t _cols_num;
    };


public:
    PerreaultFilter( const uint32_t filter_size, const cv::Mat& src, cv::Mat& res) : 
                 BaseFilter( filter_size, src, res),
                 _storage( SquareField ( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize), src)
    {
        SquareField field_new_res( kDataPtrRes, kImageWidth, _cur_x, _cur_y, kFilterSize);
        field_new_res.get_central() = _storage.getMedian();
    }
    
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
            SquareField field_new_res( kDataPtrRes, kImageWidth, _cur_x, _cur_y, kFilterSize);
            _storage.clear();
            _storage.init( field_new, _cur_y);
            field_new_res.get_central() = _storage.getMedian();
            return;
        }

        SquareField field_new( kDataPtr, kImageWidth, _cur_x, _cur_y, kFilterSize);
        SquareField field_new_res( kDataPtrRes, kImageWidth, _cur_x, _cur_y, kFilterSize);
        _storage.update( field_old.size(), _cur_x, _cur_y);
        field_new_res.get_central() = _storage.getMedian();
    }
private:
    Storage _storage;
};

cv::Mat PerreaultMedianFilter( const cv::Mat& init_image, const uint32_t filter_size)
{
    cv::Mat result = init_image.clone();
    traverse<PerreaultFilter>( init_image, result, filter_size);
    return result;
}
