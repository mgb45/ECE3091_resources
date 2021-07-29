cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ece3091_robot LANGUAGES CXX VERSION 0.01)

set(CMAKE_CXX_FLAGS "-std=c++17 -march=native -O3 -Wall -Wfatal-errors")

# Find the OpenCV package you installed, this will give you variables that you can use later
find_package(OpenCV)

# Compiling the Camera Test Program
# Create a new executable and add the required cpp files
add_executable (camera_test camera_cap.cpp)

# Link the camera test executable with the opencv libraries
target_link_libraries(
    camera_test
    ${OpenCV_LIBS}
)

# Include the header files for opencv
target_include_directories(
    camera_test PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

# Compiling the GPIO Test Program
# Create a new executable and add the required cpp files
add_executable (gpio_test gpio_test.cpp)

# Link against the gpiod library
target_link_libraries(
    gpio_test
    gpiodcxx
)

# Compiling the GPIO Test Program
# Create a new executable and add the required cpp files
add_executable (pwm_test pwm_test.cpp)

# Link against c++ filesystem (might be in experimental depending on gcc version)
target_link_libraries(
    pwm_test
    stdc++fs
)
