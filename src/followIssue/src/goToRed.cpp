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

int iLowH = 0;
int iHighH = 9;

int iLowS = 191;
int iHighS = 255;

int iLowV = 134;
int iHighV = 187;

cv::Mat getImg(const sensor_msgs::msg::Image::ConstSharedPtr& img)
{
    return cv_bridge::toCvCopy(img)->image;
}

void pointToObject(double x, double y, double imSizeW, double imSizeH)
{
    double relX = x - (imSizeW/2.0);
    //double relY = y - (imSizeH/2.0);
    double pX = (relX/(imSizeW/2.0))*100.0;
    geometry_msgs::msg::Twist movement;
    if(pX < -25.0)
    {
        movement.angular.z = 0.25;
    } else if (pX > 25.0)
    {
        movement.angular.z = -0.25;
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
    //dilate(imgEroded, imgEroded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    //erode(imgEroded, imgEroded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    Moments moment = moments(imgEroded);

    double M01 = moment.m01;
    double M10 = moment.m10;
    double Area = moment.m00;

    if(Area > 10000)
    {
        double posX = M10 / Area;
        double posY = M01 / Area;

        int size = 4;//sqrt(Area / M_PI);
        auto s = imgOriginal.size();
        auto w = s.width;
        auto h = s.height;
        Point2f center;
        double radius = 0;
        pointToObject(posX,posY,w,h);
        circle(imgOriginal, Point(posX, posY), size, Scalar(0, 0, 255));
		
		double relX = posX - (w/2.0);
		//double relY = y - (imSizeH/2.0);
		double pX = (relX/(w/2.0))*100.0;
		
		string val = "px:" + to_string(pX);
		int font = FONT_HERSHEY_SCRIPT_SIMPLEX;
		Size textSize = getTextSize(val, font, 0.2, 1, 0);
		Point textLoc(posX,posY + textSize.height);
		putText(imgOriginal, val, textLoc, font, 0.2, Scalar::all(75), 1, 8);
    }
    vw.write(imgOriginal);
    imshow("Display", imgOriginal);
	imshow("Control", imgEroded);
	waitKey(30);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc,argv);
    cout << "Init OpenCV: " << CV_VERSION << endl;
    namedWindow("Control", WINDOW_AUTOSIZE);
    //Create trackbars in "Control" window
	  createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	  createTrackbar("HighH", "Control", &iHighH, 179);

	  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	  createTrackbar("HighS", "Control", &iHighS, 255);

	  createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	  createTrackbar("HighV", "Control", &iHighV, 255);

	waitKey(30);
    auto node_ = rclcpp::Node::make_shared("follower");
    rmw_qos_profile_t custom_qos = rmw_qos_profile_default;
    auto sub = image_transport::create_subscription(node_.get(),"/image_raw",displayImage,"compressed",custom_qos);
    movePub = node_->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    vw = VideoWriter("Test.avi",VideoWriter::fourcc('M','J','P','G'),10,Size(640,480),true);
    rclcpp::spin(node_);
    rclcpp::shutdown();
    vw.release();
    return 0;
}
