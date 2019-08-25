#ifndef LAUNCHPAD_H
#define LAUNCHPAD_H

#include "ros/ros.h"
#include "serial/serial.h"
#include "serial_parser/serial_parser.h"

namespace launchpad {
class Launchpad {
 private:
  ros::NodeHandle *nh_;
  ros::NodeHandle private_nh_;
  serial::Serial *serial_;
  serial::SerialParser *serial_parser;

  double rate_;
  int data_size_;
  //double hercules_data;
  std::string start_delimitter_;
  std::string end_delimitter_;
  std::string pub_topic_;

 public:
  Launchpad(ros::NodeHandle *node, serial::Serial *serial);
  void run();
};
}
#endif
