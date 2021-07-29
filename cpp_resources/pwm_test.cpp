// Example on how to use sysfs interface for hardware pwm
#include <chrono>
#include <thread>

#include "pwm_chip.hpp"

void test_pwm(pwmchip& pwm)
{
    using namespace std::chrono_literals;

    pwm.set_period(2e7); // 1ms period
    pwm.set_duty_cycle(0.5); // 50% duty cycle

    pwm.enable();
    std::cout << "Enabling PWM for 2s" << std::endl;
    std::this_thread::sleep_for(2s);

    pwm.disable();
    std::cout << "Disabling PWM for 2s" << std::endl;
    std::this_thread::sleep_for(2s);

    pwm.enable();
    std::cout << "Enabling PWM for 2s" << std::endl;
    std::this_thread::sleep_for(2s);

    pwm.disable();
    std::cout << "PWM Test Complete" << std::endl;
}

int main(int argc, char** argv)
{
    pwmchip pwm_0(0, 0); // Chip 0, pwm idx 0
    pwmchip pwm_1(0, 1); // Chip 0, pwm idx 1

    test_pwm(pwm_0);
    test_pwm(pwm_1);

    return 0;
}
