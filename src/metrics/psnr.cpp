
#include <metrics/mse.hpp>

double psnr( const cv::Mat& image1, const cv::Mat& image2)
{
    const uint64_t MAX = 255;
    const double first = 20 * log10( MAX);
    const double second = 10 * mse( image1, image2).log10();

    return first - second;
}