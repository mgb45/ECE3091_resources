// Example on how to use sysfs interface for hardware pwm
#include <chrono>
#include <thread>

#include "pwm_chip.hpp"

int main(int argc, char** argv)
{
    using namespace std::chrono_literals;

    pwmchip pwm_0(0, 0); // Chip 0, pwm idx 0
    pwm_0.set_period(5e8); // 500ms period
    pwm_0.set_duty_cycle(0.5); // 50% duty cycle

    pwm_0.enable();
    std::cout << "Enabling PWM for 2s" << std::endl;
    std::this_thread::sleep_for(2s);

    pwm_0.disable();
    std::cout << "Disabling PWM for 2s" << std::endl;
    std::this_thread::sleep_for(2s);

    pwm_0.enable();
    std::cout << "Enabling PWM for 2s" << std::endl;
    std::this_thread::sleep_for(2s);

    std::cout << "PWM Test Complete" << std::endl;
    return 0;
}
