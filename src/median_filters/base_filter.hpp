#include <inttypes.h>
#include <pixel/pixel.hpp>
#include <opencv2/highgui.hpp>

class BaseFilter
{
public:
    explicit BaseFilter( const uint32_t filter_size, cv::Mat& src) : kFilterSize( filter_size),
                                                                     kImageWidth( src.cols),
                                                                     kImageHeight( src.rows),
                                                                     kDataPtr( (RGB_pixel*)src.data),
                                                                     _cur_x( filter_size / 2),
                                                                     _cur_y( filter_size / 2) {}
    void step() = delete;

protected:
    const uint32_t kFilterSize;
    const uint32_t kImageWidth;
    const uint32_t kImageHeight;
    RGB_pixel* const kDataPtr;
    uint32_t _cur_x;
    uint32_t _cur_y;
};
