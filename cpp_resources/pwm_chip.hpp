#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#if defined(__GNUC__) && __GNUC__ > 7
#include <filesystem>
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#endif

class pwmchip {
public:

    // Constructor that takes the chip index and pwm channel index
    pwmchip(unsigned int chip_idx, unsigned int channel_idx) :
        m_fs_path{"/sys/class/pwm/pwmchip" + std::to_string(chip_idx)},
        m_channel_index{channel_idx},
        m_pwm_pth{m_fs_path + "/pwm" + std::to_string(channel_idx)}
    {
        if (filesystem::exists(m_pwm_pth)) {
            throw std::runtime_error(m_pwm_pth + " already enabled, someone else may already be controlling it");
        }
        std::string export_pth = m_fs_path + "/export";
        std::ofstream write_file(export_pth, std::ios::binary);
        write_file << m_channel_index;
        write_file.close();
        if (write_file.bad() || !filesystem::exists(m_pwm_pth)) {
            throw std::runtime_error("Error exporting " + m_pwm_pth);
        }
        this->disable(); // Start with PWM disabled
    }

    ~pwmchip()
    {
        if (!filesystem::exists(m_pwm_pth)) {
            std::cerr << m_pwm_pth  << " isn't enabled during pwmchip destruction";
        }
        this->disable(); // Disable PWM output
        std::string unexport_pth = m_fs_path + "/unexport";
        std::ofstream write_file(unexport_pth, std::ios::binary);
        write_file << m_channel_index;
        write_file.close();
        if (write_file.bad() || filesystem::exists(m_pwm_pth)) {
            std::cerr << "Error unexporting " << m_pwm_pth;
        }
    }

    // There should only be one instance, don't want to be exporting/unexporting by accident.
    pwmchip(const pwmchip& other) = delete; // No copy constructor
    pwmchip(pwmchip&& other) = delete; // No move constructor
    pwmchip& operator=(const pwmchip& other) = delete; // No copy assignment
    pwmchip& operator=(pwmchip&& other) = delete; // No move assignment

    // Set the period of the duty cycle in nanoseconds
    void set_period(unsigned int nanoseconds)
    {
        std::string period_pth = m_pwm_pth + "/period";
        std::ofstream write_file(period_pth, std::ios::binary);
        write_file << nanoseconds;
        m_period = nanoseconds;
        write_file.close();
        if (write_file.bad()) {
            std::cerr << "Error setting period " << m_pwm_pth;
        }
    }

    // Duty cycle is the requred percentage as a fraction between 0 and 1
    void set_duty_cycle(float percentage)
    {
        assert((percentage >= 0.f && percentage <= 1.f) && "duty cycle should be in range [0, 1]");
        std::string period_pth = m_pwm_pth + "/duty_cycle";
        std::ofstream write_file(period_pth, std::ios::binary);
        write_file << static_cast<int>(percentage * static_cast<float>(m_period));
        write_file.close();
        if (write_file.bad()) {
            std::cerr << "Error setting duty cycle " << m_pwm_pth;
        }
    }

    // Enable the PWM Signal
    void enable()
    {
        std::string period_pth = m_pwm_pth + "/enable";
        std::ofstream write_file(period_pth, std::ios::binary);
        write_file << 1;
        write_file.close();
        if (write_file.bad()) {
            std::cerr << "Enabling PWM " << m_pwm_pth;
        }
    }

    // Disable the PWM Signal
    void disable()
    {
        std::string period_pth = m_pwm_pth + "/enable";
        std::ofstream write_file(period_pth, std::ios::binary);
        write_file << 0;
        write_file.close();
        if (write_file.bad()) {
            std::cerr << "Disabling PWM " << m_pwm_pth;
        }
    }

private:
    const std::string m_fs_path;
    const int m_channel_index;
    const std::string m_pwm_pth;
    int m_period;
};
