#include<iostream>
#include"rclcpp/rclcpp.hpp"
#include<image_transport/image_transport.h>
#include<opencv2/core.hpp>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

VideoWriter vw;

void displayImage(const sensor_msgs::msg::Image::ConstSharedPtr& img)
{
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(img);
        cv::imshow("Display",cv_ptr->image);
        vw.write(cv_ptr->image);
    }
    catch (cv_bridge::Exception& e)
    {
        cout << "Error converting: " << e.what() << endl;
        return;
    }
}


int main(int argc, char* argv[])
{
    rclcpp::init(argc,argv);
    cout << "Init: " << CV_VERSION << endl;
    auto node_ = rclcpp::Node::make_shared("videoReader");
    rmw_qos_profile_t custom_qos = rmw_qos_profile_default;
    auto sub = image_transport::create_subscription(node_.get(),"/image_raw",displayImage,"compressed",custom_qos);
    vw = VideoWriter("Test.avi",VideoWriter::fourcc('M','J','P','G'),24,Size(640,480),true);
    rclcpp::spin(node_);
    rclcpp::shutdown();
    vw.release();
    return 0;
}
