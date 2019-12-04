#define _USE_MATH_DEFINES
#include <math.h>
#include<iostream>
#include <chrono>
#include"rclcpp/rclcpp.hpp"
#include<image_transport/image_transport.h>
#include<opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.hpp>
#include<opencv2/highgui.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include "std_msgs/msg/string.hpp"

using namespace std;
using namespace cv;
using namespace chrono;

VideoWriter vw;
rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr movePub;

int iLowH = 162;
int iHighH = 179;

int iLowS = 193;
int iHighS = 255;

int iLowV = 153;
int iHighV = 255;

cv::Mat getImg(const sensor_msgs::msg::Image::ConstSharedPtr& img)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(img);
    return cv_ptr->image;
}

void pointToObject(double x, double y, double imSizeW, double imSizeH)
{
    double relX = x - (imSizeW/2.0);
    double relY = y - (imSizeH/2.0);
    double pX = (relX/(imSizeW/2.0))*100.0;
    geometry_msgs::msg::Twist movement;
    if(pX < -50.0)
    {
        movement.angular.z = -0.1;
    } else if (pX > 50.0)
    {
        movement.angular.z = 0.1;
    } else
    {
        movement.angular.z = 0.0;
    }

    movePub->publish(movement);
}

void displayImage(const sensor_msgs::msg::Image::ConstSharedPtr& img)
{
    Mat imgOriginal = getImg(img);
    Mat imgHSV;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
    //GaussianBlur(imgHSV, imgHSV, Size(3, 3), 0);
    Mat imgThresholded;
    Mat imgEroded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgEroded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgEroded, imgEroded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //morphological closing (fill small holes in the foreground)
    dilate(imgEroded, imgEroded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgEroded, imgEroded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    Moments moment = moments(imgEroded);

    double M01 = moment.m01;
    double M10 = moment.m10;
    double Area = moment.m00;

    if(Area > 10000)
    {
        double posX = M10 / Area;
        double posY = M01 / Area;

        int size = sqrt(Area / M_PI);
        auto s = imgOriginal.size();
        auto w = s.width;
        auto h = s.height;
        Point2f center;
        double radius = 0;
        pointToObject(posX,posY,w,h);
        circle(imgOriginal, Point(posX, posY), size, Scalar(0, 0, 255));
    }
    vw.write(imgOriginal);
    imshow("Display", imgOriginal);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc,argv);
    cv::namedWindow("Display",WINDOW_AUTOSIZE);
    cout << "Init OpenCV: " << CV_VERSION << endl;
    auto node_ = rclcpp::Node::make_shared("follower");
    rmw_qos_profile_t custom_qos = rmw_qos_profile_default;
    auto sub = image_transport::create_subscription(node_.get(),"/image_raw",displayImage,"compressed",custom_qos);
    movePub = node_->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    vw = VideoWriter("Test.avi",VideoWriter::fourcc('M','J','P','G'),24,Size(640,480),true);
    rclcpp::spin(node_);
    rclcpp::shutdown();
    vw.release();
    return 0;
}