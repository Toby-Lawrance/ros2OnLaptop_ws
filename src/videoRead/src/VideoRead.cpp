#include<iostream>
#include"rclcpp/rclcpp.hpp"
#include<image_transport/image_transport.h>
#include<opencv2/core.hpp>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void displayImage(const sensor_msgs::msg::Image::ConstSharedPtr& img)
{
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        cout << "Error converting: " << e.what() << endl;
        return;
    }

    imshow("Display",cv_ptr->image);
}

int main(int argc, char* argv[])
{
    namedWindow("Display",WINDOW_AUTOSIZE);
    rclcpp::init(argc,argv);
    cout << "Init" << endl;
    auto node_ = rclcpp::Node::make_shared("videoReader");
    rmw_qos_profile_t custom_qos = rmw_qos_profile_default;
    auto sub = image_transport::create_subscription(node_.get(),"/image_raw",displayImage,"theora",custom_qos);
    rclcpp::spin(node_);
    return 0;
}

