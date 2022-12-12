#pragma once

#include <opencv2/highgui.hpp>
#include <rotate/bilinear.hpp>

cv::Mat FastHoghTransform( const cv::Mat& init, cv::Mat(*rotationFuncton)(const cv::Mat&, double) = bilinearRotate, int num = 1);

cv::Mat FastHoughAlgorithm( const cv::Mat& image);
