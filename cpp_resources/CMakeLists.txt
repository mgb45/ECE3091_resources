cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(ece3091_robot LANGUAGES CXX VERSION 0.01)

set(CMAKE_CXX_FLAGS "-std=c++17 -march=native -O3 -Wall -Wfatal-errors")

find_package(OpenCV)

add_executable (camera_test camera_cap.cpp)

target_link_libraries(
    camera_test
    ${OpenCV_LIBS}
)

target_include_directories(
    camera_test PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

add_executable (gpio_test gpio_test.cpp)
