#include <FHT/FastHoughTransform.hpp>
#include <rotate/nearest_neighbour.hpp>
#include <rotate/bilinear.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <cmath>

static cv::Mat SobelOperator( const cv::Mat& image)
{
    cv::Mat blurred, gray, grad;

    // Remove noise by blurring with a Gaussian filter
    cv::GaussianBlur( image, blurred, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    // Convert to grayscale
    cv::cvtColor( blurred, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, abs_grad_x, grad_y, abs_grad_y;
    cv::Sobel( gray, grad_x, CV_16S, 1, 0);
    cv::Sobel( gray, grad_y, CV_16S, 0, 1);

    // Convert to CV_8U for future work
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    return grad;
}

static cv::Mat PadToTwoPower( const cv::Mat& image)
{
    cv::Mat res;
    uint32_t max_param = image.cols;
    uint32_t new_size = std::pow( 2, std::ceil( std::log2( max_param)));
    uint32_t width_delta = (new_size - image.cols);
    cv::copyMakeBorder( image, res, 0, 0,  width_delta / 2, (width_delta + 1) / 2, cv::BORDER_CONSTANT, 0);
    return res;
}

static std::pair<double, uint32_t> getMaxLineVar( const cv::Mat& image)
{
    double max_stddev = 0;
    uint32_t index = 0;

    for ( int i = 0; i < image.cols; ++i )
    {
        cv::Scalar mean, stddev;
        cv::meanStdDev( image.col( i), mean, stddev);
        double cur_stddev = stddev.val[0];
        if ( cur_stddev > max_stddev )
        {
            index = i;
            max_stddev = cur_stddev;
        }
    }
    return std::make_pair( max_stddev * max_stddev, index);
}

static cv::Mat calc_sums( cv::Mat& image, uint32_t xmin, uint32_t xmax)
{

    uint32_t height = image.rows;
    int32_t stripe_width = xmax - xmin;
    cv::Mat res( height, stripe_width, CV_32SC1);
    if ( stripe_width == 1 )
    {
        for ( int32_t y = 0; y < height; ++y )
        {
            res.at<int32_t>( y, 0) = image.at<int32_t>( y, xmin);
        }
    } else
    {
        uint32_t xmid = (xmin + xmax) / 2;
        cv::Mat answer1 = calc_sums( image, xmin, xmid);
        cv::Mat answer2 = calc_sums( image, xmid, xmax);

        for ( int32_t y = 0; y < height; ++y )
        {
            for ( int32_t shift = 0; shift < stripe_width; ++shift )
            {
                res.at<int32_t>( y, shift) = answer1.at<int32_t>( y, shift / 2) +
                                             answer2.at<int32_t>( (y + shift / 2 + shift % 2) % height, shift / 2);
            }
        }
    }
    return res;

    // uint32_t height = image.rows;
    // if ( xmax - xmin == 1 )
    // {
    //     return image;
    // } else
    // {
    //     uint32_t xmid = (xmin + xmax) / 2;
    //     calc_sums( image, xmin, xmid);
    //     calc_sums( image, xmid, xmax);
    //     int32_t stripe_width = xmax - xmin;
    //     for ( int32_t y = 0; y < height; ++y )
    //     {
    //         for ( int32_t shift = (stripe_width - 1); shift >= 0; --shift )
    //         {
    //             int32_t second = 0;
    //             if ( y + shift / 2 + shift % 2 < height)
    //             {
    //                 second = image.at<int32_t>( (y + shift / 2 + shift % 2)/* % height*/, xmin + stripe_width / 2 + shift / 2);
    //             }
    //             image.at<int32_t>( y, xmin + shift) = image.at<int32_t>( y, xmin + shift / 2) +
    //                                                   second;
    //         }
    //     }
    // }
    // return image;
}

static cv::Mat FastHoughAlgorithm( cv::Mat& image)
{
    return calc_sums( image, 0, image.cols);
}

static cv::Mat normalizeMat( cv::Mat& image)
{
    double minVal; 
    double maxVal; 
    cv::Point minLoc; 
    cv::Point maxLoc;
    cv::minMaxLoc( image, &minVal, &maxVal, &minLoc, &maxLoc );

    cv::Mat result;
    double coef = 255.0 / (maxVal - minVal);
    image.convertTo( result, CV_8U, coef, -minVal * coef);
    return result;
}

cv::Mat FastHoghTransform( const cv::Mat& init, cv::Mat(*rotationFuncton)(const cv::Mat&, double), int num)
{
    cv::Mat grad_image = SobelOperator( init);
    cv::Mat grad_images[2] = { grad_image, SobelOperator( rotationFuncton( init, M_PI_2))};

    auto name = std::string("examples/FHT/transformed/") + 
                std::to_string(num) + "_grad.jpg";
    cv::imwrite( name, grad_image);

    std::pair<double, uint32_t> var_info[2];
    double angle = 0;

    for ( int i = 0; i < 2; ++i )
    {
        cv::Mat padded = PadToTwoPower( grad_images[i]);
        cv::Mat int_image;
        padded.convertTo( int_image, CV_32SC1);

        cv::Mat res = FastHoughAlgorithm( int_image);

        if ( i == 0 )
        {
            auto name = std::string("examples/FHT/transformed/") + 
            std::to_string(num) + "_hough.jpg";
            cv::imwrite( name, normalizeMat(res));
        }

        var_info[i] = getMaxLineVar( res);
    }



    if ( var_info[1].first > var_info[0].first)
    {
        angle = -std::atan( (double)var_info[1].second / grad_images[1].cols);
    } else
    {
        angle = std::atan( (double)var_info[0].second / grad_images[0].cols);
    }

    std::cout << angle << "\n";
    cv::Mat rotated = rotationFuncton( init, angle);
    //return res;
    return rotated;
}