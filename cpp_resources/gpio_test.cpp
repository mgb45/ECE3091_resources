// Example on how to use libgpio for chardev interface (sysfs is deprecated)

#include <chrono>
#include <iostream>
#include <thread>

#include <gpiod.hpp>

int main(int argc, char** argv)
{
    auto chip = gpiod::chip();

    chip.open("/dev/gpiochip0", gpiod::chip::OPEN_BY_PATH);

    auto gpio_pin = chip.get_line(26); // "GPIO 26" or Pin 37 on raspberry pi board
    auto request = gpiod::line_request();
    request.request_type = gpiod::line_request::DIRECTION_OUTPUT;
    gpio_pin.request(request);
    
    for (std::size_t i=0; i<4; ++i)
    {
        using namespace std::chrono_literals;
        auto current_state = gpio_pin.get_value();
        std::cout << "Setting Logic State " << !current_state << std::endl;
        gpio_pin.set_value(!current_state);
        std::this_thread::sleep_for(5000ms);
    }

    std::cout << "GPIO Test Completed" << std::endl;
}
