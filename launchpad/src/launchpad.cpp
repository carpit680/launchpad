#include "launchpad/launchpad.h"
#include "ros/ros.h"
#include "serial/serial.h"
#include <std_msgs/String.h>
#include <iostream>


using namespace launchpad;

Launchpad::Launchpad(ros::NodeHandle *node, serial::Serial *serial)
    : nh_(node), private_nh_("~"), serial_(serial) {
  private_nh_.param("rate", rate_, 30.0);
  private_nh_.param("data_size", data_size_, 6);
  private_nh_.param("start_delimitter", start_delimitter_, std::string("["));
  private_nh_.param("end_delimitter", end_delimitter_, std::string("]"));
  private_nh_.param("pub_topic", pub_topic_, std::string("/ultrasonic/range"));
}

void Launchpad::run() {
  ros::Rate loop_rate(rate_);
  auto last_update_time = ros::Time::now();
  this->serial_parser = new serial::SerialParser(*serial_, start_delimitter_,
                                                 end_delimitter_, data_size_);
  std::string parsed_string;
  auto *buffer = (uint8_t *)malloc(sizeof(uint8_t) * data_size_);
  auto buffer_length = (size_t)data_size_;
  ros::Publisher pub = nh_->advertise<std_msgs::String>(pub_topic_, 1000);
  std_msgs::String msg;
  while (ros::ok()) {
    parsed_string = this->serial_parser->get_parsed_string();
    //std::cout << parsed_string;
    //int c = int(parsed_string);
    //msg.data = c;
    msg.data = parsed_string;
    if (!parsed_string.empty()) {
      
      pub.publish(msg);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
}
