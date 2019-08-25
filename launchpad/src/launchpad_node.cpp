#include "launchpad/launchpad.h"
#include "ros/ros.h"
#include "serial/serial.h"

std::string port;
unsigned long baud;
//static double control_effort = 0.0;

void serial_sleep(unsigned long milliseconds) {
  usleep(milliseconds * 1000);  // 100 ms
}

void enumerate_ports() {
  std::vector<serial::PortInfo> devices_found = serial::list_ports();

  auto iter = devices_found.begin();

  while (iter != devices_found.end()) {
    serial::PortInfo device = *iter++;
    printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
           device.hardware_id.c_str());
  }
}

void print_usage() {
  std::cerr << "No port or baudrate specified to connect to device";
  std::cerr << "Usage: test_serial {-e|<serial port address>} <baudrate>";
}

bool parse_arguments(int argc, char** argv) {
  if (argc < 2) {
    print_usage();
    return false;
  }

  // Argument 1 is the serial port or enumerate flag
  port = std::string(argv[1]);
  if (port == "-e") {
    enumerate_ports();
    return false;
  } else if (argc < 3) {
    print_usage();
    return true;
  }

  // Argument 2 is the baudrate
  baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
  sscanf_s(argv[2], "%lu", &baud);
#else
  sscanf(argv[2], "%lu", &baud);
#endif
  return true;
}

bool check_connection(serial::Serial* serial) {
  std::cout << "Is the serial port open?";
  if (serial->isOpen()) {
    std::cout << " Yes." << std::endl;
    return true;
  } else {
    std::cout << " No." << std::endl;
    return false;
  }
}

void run() {
  // port, baudrate, timeout in milliseconds
  serial::Serial serial(port, baud, serial::Timeout::simpleTimeout(1000));
  ros::NodeHandle node;

  if (check_connection(&serial)) {
    //std::cout << "test" << std::endl;
    auto launchpad =
        new launchpad::Launchpad(&node, &serial);
    
    launchpad->run();
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "launchpad");
  try {
    if (parse_arguments(argc, argv)) run();
  } catch (std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
  }

  return 0;
}
