// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <math.h>
#include <signal.h>

#include <geometry_msgs/msg/twist.hpp>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std;
using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

void h_sig_sigint(int signum)
{
	std::cout << "Receive signum: " << signum << std::endl;
	rclcpp::shutdown();
}


class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("square_mover")
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    for(;;)
    {
      travel(0.2,0.15);
      rotate(90,0.25);
    }
  }

  void travel(double distance, double speed = 0.1)
  {
    string travelling = "Moving: " + to_string(distance) + "m at " + to_string(speed) + "m/s";
    RCLCPP_INFO(this->get_logger(),travelling);
    geometry_msgs::msg::Twist movement;
    movement.linear.x = speed;
    movement.angular.z = 0.0;

    publisher_->publish(movement);

    double waitTimeS = distance/speed;
    long waitTime = (long)(waitTimeS*1000.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));

    geometry_msgs::msg::Twist stop;
    stop.linear.x = 0.0;
    stop.angular.z = 0.0;
    publisher_->publish(stop);
  }

  void rotate(double angle, double speed = 0.1)
  {
      string travelling = "turning: " + to_string(angle) + "deg at " + to_string(speed) + "m/s";
      RCLCPP_INFO(this->get_logger(),travelling);
      double radAngle = angle * (M_PI/180.0);
      double time = abs(radAngle) / speed;
      long waitTime = (long)(time * 1000);

      geometry_msgs::msg::Twist movement;
      movement.linear.x = 0.0;
      movement.angular.z = angle > 0 ? -speed : speed;

      publisher_->publish(movement);

      std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));

      geometry_msgs::msg::Twist stop;
      stop.linear.x = 0.0;
      stop.angular.z = 0.0;
      publisher_->publish(stop);
  }

private:
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  signal(SIGINT, h_sig_sigint);
  signal(SIGKILL, h_sig_sigint);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
