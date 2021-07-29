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
        std::ofstream write_file(export_pth);
        write_file << m_channel_index;
        write_file.close();
        if (write_file.bad() || !filesystem::exists(m_pwm_pth)) {
            throw std::runtime_error("Error exporting " + m_pwm_pth);
        }
        this->disable(); // Start with PWM disabled
        m_duty_ns = this->get_duty_cycle();
        m_period_ns = this->get_period();
        m_duty_percent = static_cast<float>(m_duty_ns) / static_cast<float>(m_period_ns);
    }

    ~pwmchip()
    {
        if (!filesystem::exists(m_pwm_pth)) {
            std::cerr << m_pwm_pth  << " isn't enabled during pwmchip destruction";
        }
        this->disable(); // Disable PWM output before unexport
        std::string unexport_pth = m_fs_path + "/unexport";
        std::ofstream write_file(unexport_pth);
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
    void set_period(unsigned int nanoseconds) noexcept
    {
        m_period_ns = nanoseconds;
        m_duty_ns = static_cast<unsigned int>(m_duty_percent * static_cast<float>(m_period_ns));

        // Write the parameters to the files in the correct order
        if (this->get_period() > m_period_ns) {
            this->write_duty_cycle();
            this->write_period();
        } else {
            this->write_period();
            this->write_duty_cycle();
        }
    }

    // Read period currently stored in sysfs
    [[nodiscard]] unsigned int get_period() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/period";
        std::ifstream read_file(ctrl_pth);
        unsigned int f_period;
        read_file >> f_period;
        read_file.close();
        return f_period;
    }

    // Duty cycle is the requred percentage as a fraction between 0 and 1
    void set_duty_cycle(float percentage) noexcept
    {
        assert((percentage >= 0.f && percentage <= 1.f) && "duty cycle should be in range [0, 1]");
        m_duty_percent = percentage;
        m_duty_ns = static_cast<unsigned int>(m_duty_percent * static_cast<float>(m_period_ns));
        this->write_duty_cycle();
    }

    // Read duty cycle currently stored in sysfs
    [[nodiscard]] unsigned int get_duty_cycle() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/duty_cycle";
        std::ifstream read_file(ctrl_pth);
        unsigned int f_duty;
        read_file >> f_duty;
        read_file.close();
        return f_duty;
    }

    // Read enable status currently stored in sysfs
    [[nodiscard]] bool get_status() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/enable";
        std::ifstream read_file(ctrl_pth);
        bool status;
        read_file >> status;
        read_file.close();
        return status;
    }

    // Enable the PWM Signal
    void enable() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/enable";
        std::ofstream write_file(ctrl_pth);
        write_file << 1;
        write_file.close();
        if (!this->get_status()) {
            std::cerr << "Error Enabling PWM " << ctrl_pth << "\n";
        }
    }

    // Disable the PWM Signal
    void disable() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/enable";
        std::ofstream write_file(ctrl_pth);
        write_file << 0;
        write_file.close();
        if (this->get_status()) {
            std::cerr << "Error Disabling PWM " << ctrl_pth << "\n";
        }
    }

private:
    const std::string m_fs_path;
    const int m_channel_index;
    const std::string m_pwm_pth;
    unsigned int m_period_ns;
    unsigned int m_duty_ns;
    float m_duty_percent;

    // Write the target duty cycle to the file
    void write_duty_cycle() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/duty_cycle";
        std::ofstream write_file(ctrl_pth);
        write_file << m_duty_ns;
        write_file.close();
        if (this->get_duty_cycle() != m_duty_ns) {
            std::cerr << "Error setting duty cycle " << m_duty_ns << ": " << ctrl_pth << "\n";
        }
    }

    // Write the target period to the file
    void write_period() noexcept
    {
        const std::string ctrl_pth = m_pwm_pth + "/period";
        std::ofstream write_file(ctrl_pth, std::ios::binary);
        write_file << m_period_ns;
        write_file.close();
        if (this->get_period() != m_period_ns) {
            std::cerr << "Error setting period " << m_period_ns << ": "<< ctrl_pth << "\n";
        }
    }

};
