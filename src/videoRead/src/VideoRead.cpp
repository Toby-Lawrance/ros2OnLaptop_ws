#include<iostream>
#include <image_transport/image_transport.h>
#include<opencv2/core.hpp>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>

using namespace std;
using namespace cv;

class videoReader : public rclcpp::Node
{
public:
    videoReader() : Node("videoReader")
    {
        sub = this->create_subscription<sensor_msgs::ImageConstPtr>("camera/image_raw", 1, bind(&videoReader::displayImage,this, 1));
    }

private:
    void displayImage(const sensor_msgs::ImageConstPtr img)
    {
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8)
        } catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        imshow("Display",cv_ptr->image);
    }
    rclcpp::Subscription<sensor_msgs::ImageConstPtr>::SharedPtr sub;
};

int main(int argc, char* argv[])
{
    namedWindow("Display",WINDOW_AUTOSIZE);
    rclcpp::init(argc,argv);
    rclcpp::spin(make_shared<videoReader>());
    rclcpp::shutdown();
    return 0;
}

