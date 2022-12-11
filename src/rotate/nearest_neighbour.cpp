#include <rotate/nearest_neighbour.hpp>
#include <cmath>

cv::Mat nearestNeighbourRotate( const cv::Mat& src, const double angle)
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

            int32_t rounded_x = std::round( src_point.x);
            int32_t rounded_y = std::round( src_point.y);
            if ( rounded_x >= 0 && rounded_x < width && rounded_y >= 0 && rounded_y < height )
            {
                // dst.at<cv::Vec3b>( y, x) = src.at<cv::Vec3b>( rounded_y, rounded_x);
                memcpy( &dst.data[(y * dst.cols + x) * dst.channels()], 
                        &src.data[(rounded_y * src.cols + rounded_x) * src.channels()],
                        src.channels());
            }
        }
    }
    return dst;
}