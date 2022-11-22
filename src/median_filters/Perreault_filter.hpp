#pragma once

#include <opencv2/highgui.hpp>

cv::Mat PerreaultMedianFilter( const cv::Mat& init_image, const uint32_t filter_size);
