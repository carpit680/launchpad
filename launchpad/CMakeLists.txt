cmake_minimum_required(VERSION 2.8.3)
project(launchpad)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  serial_parser
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES launchpad
  CATKIN_DEPENDS roscpp serial serial_parser std_msgs
  ##//DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} src/${PROJECT_NAME}.cpp)
add_executable(${PROJECT_NAME}_node src/${PROJECT_NAME}_node.cpp)
target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME} ${catkin_LIBRARIES})

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)
