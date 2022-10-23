#pragma once

#include <pixel/pixel.hpp>
#include <opencv2/highgui.hpp>

colour get_bayer_colour( uint32_t i, uint32_t j);
cv::Mat make_rgb_bayer( const cv::Mat& src);
