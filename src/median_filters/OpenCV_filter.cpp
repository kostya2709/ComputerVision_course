#include <opencv2/opencv.hpp>

cv::Mat OpenCVMedianFilter( const cv::Mat& init_image, uint32_t filter_size)
{
    cv::Mat result = init_image.clone();
    if ( filter_size % 2 == 0 )
    {
        ++filter_size;
    }
    cv::medianBlur(init_image, result, filter_size);
    return result;
}
