#include <iostream>
#include <chrono>
#include <stdexcept>

#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

void camera_capture_example(int camera_id)
{
    auto camera = cv::VideoCapture(camera_id, cv::CAP_V4L2);
    
    if (!camera.isOpened()){
        throw std::runtime_error({"Error creating camera capturing object with camera ID: ", camera_id});
    }

    auto start_time = std::chrono::system_clock::now();

    cv::Mat captured_image;
    camera >> captured_image;

    cv::imwrite("test_image.png", captured_image);

    const auto shape = captured_image.size;
    std::cout << "Image Dimensions: " << shape[0] << "x" << shape[1] << "\n";
    auto video_writer = cv::VideoWriter("sample.mp4", cv::VideoWriter::fourcc('m','p','4','v'), 25., {shape[1], shape[0]});

    if (!video_writer.isOpened()){
        throw std::runtime_error("Video writer initialization error");
    }

    video_writer.write(captured_image);

    while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time).count() < 10)
    {
        camera >> captured_image;
        video_writer.write(captured_image);
    }

    camera.release();
    video_writer.release();

}

int main(int argc, char** argv)
{
    camera_capture_example(-1);
    std::cout << "Camera Test Completed" << std::endl;
}