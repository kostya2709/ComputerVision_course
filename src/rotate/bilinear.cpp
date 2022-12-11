#include <rotate/bilinear.hpp>
#include <iostream>
#include <cmath>

cv::Mat bilinearRotate( const cv::Mat& src, const double angle)
{
    double width = src.cols;
    double height = src.rows;

    const cv::Point2d centre( (double)width / 2, (double)height / 2);
    const double reverse_angle = -angle;
    
    cv::Mat_<double> rotation_matrix( 2, 2);
    rotation_matrix.at<double>( 0, 0) =  cos( reverse_angle);
    rotation_matrix.at<double>( 0, 1) =  sin( reverse_angle);
    rotation_matrix.at<double>( 1, 0) = -sin( reverse_angle);
    rotation_matrix.at<double>( 1, 1) =  cos( reverse_angle);

    const cv::Mat left_down = rotation_matrix * cv::Mat( cv::Vec2d( -width / 2, -height / 2));
    const cv::Mat right_down = rotation_matrix * cv::Mat( cv::Vec2d( width / 2, -height / 2));
    const cv::Mat left_up = rotation_matrix * cv::Mat( cv::Vec2d( -width / 2, height / 2));
    const cv::Mat right_up = rotation_matrix * cv::Mat( cv::Vec2d( width / 2, height / 2));

    const cv::Mat diag1 = right_up - left_down;
    const cv::Mat diag2 = left_up - right_down;
    const uint32_t new_width  = (uint32_t)std::max( std::abs( diag1.at<double>( 0, 0)), std::abs( diag2.at<double>( 0, 0)));
    const uint32_t new_height = (uint32_t)std::max( std::abs( diag1.at<double>( 1, 0)), std::abs( diag2.at<double>( 1, 0)));
    const cv::Point2d new_centre( new_width / 2, new_height / 2);
    cv::Mat dst( new_height, new_width, src.type(), cv::Scalar(0));

    for ( uint32_t x = 0; x < new_width; ++x )
    {
        for ( uint32_t y = 0; y < new_height; ++y )
        {
            const cv::Vec2d vec( x - new_centre.x, y - new_centre.y);
            const cv::Mat src_vec = rotation_matrix * cv::Mat( vec);
            const cv::Point2d src_point( centre.x + src_vec.at<double>( 0, 0), centre.y + src_vec.at<double>( 1, 0));

            int32_t x_arr[] = { std::floor( src_point.x), std::ceil( src_point.x)};
            int32_t y_arr[] = { std::floor( src_point.y), std::ceil( src_point.y)};
            int inside_cnt = 0;
            int sum[src.channels()];
            for ( int i = 0; i < src.channels(); ++i )
            {
                sum[i] = 0;
            }

            for ( int i = 0; i < 2; ++i )
            {
                for ( int j = 0; j < 2; ++j )
                {
                    int x_cur = x_arr[i];
                    int y_cur = y_arr[j];
                    if ( x_cur >= 0 && x_cur < width && y_cur >= 0 && y_cur < height )
                    {
                        ++inside_cnt;
                        for ( int i = 0; i < src.channels(); ++i )
                        {
                            sum[i] += src.data[(y_cur * src.cols + x_cur) * src.channels() + i];
                        }
                    }
                }
            }

            if ( inside_cnt )
            {
                for ( int i = 0; i < src.channels(); ++i )
                {
                    dst.data[(y * dst.cols + x) * dst.channels() + i] = sum[i] / inside_cnt;
                }
            }
        }
    }
    return dst;
}