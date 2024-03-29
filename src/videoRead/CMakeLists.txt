cmake_minimum_required(VERSION 3.5)
project(videoRead)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#SET(OpenCV_DIR ~/installation/OpenCV-master/lib/cmake/opencv4)

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
#find_package(OpenCV 4 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(sensor_msgs REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
add_executable( VideoRead src/VideoRead.cpp)
ament_target_dependencies(VideoRead OpenCV rclcpp cv_bridge image_transport snesor_msgs)
target_link_libraries (VideoRead ${OpenCV_LIBS} )

install(TARGETS
  VideoRead
  DESTINATION lib/${PROJECT_NAME})
