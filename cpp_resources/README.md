# Camera Notes
You can see the camera available with the command below.

```bash
v4l2-ctl --list-devices
```

See what parameters you can change for the camera, for example you may want to reduce the exposure time to reduce motion blur.

```bash
v4l2-ctl -d /dev/video0 --list-ctrls-menus
```

You can then change the properties with command line args. If you make a configuration you like, you may want to write a shell script to run all the configuration at once in one step.

```bash
v4l2-ctl --set-ctrl=auto_exposure_bias=6
```

Capture test video with the command below, only specific resolutions or framerates are supported, you will need to check this yourself.

```bash
ffmpeg -f v4l2 -framerate 25 -video_size 640x480 -i /dev/video0 output.mkv
```

# GPIO Hardware Interaction

To get the correct permisssions for the Hardware Interfaces, either you can setup a new usergroup with some perms that unlock the required interfaces, or you can just run the executable with gpio/pwm things with sudo (pls don't break anything).

## GPIO Pins

Using the gpioinfo tool you can determine the target line you need for the named pin. From this line name, you can use the example code supplied to control it, setup callbacks etc. An example output for an Nvidia Jetson Nano 2GB is shown below.

```bash
sudo gpioinfo /dev/gpiochip0 | grep -v unnamed # Skip lines without name

gpiochip0 - 256 lines:
    line  12:  "SPI1_MOSI"       unused   input  active-high 
    line  13:  "SPI1_MISO"       unused   input  active-high 
    line  14:   "SPI1_SCK"       unused   input  active-high 
    line  15:   "SPI1_CS0"       unused   input  active-high 
    line  16:  "SPI0_MOSI"       unused   input  active-high 
    line  17:  "SPI0_MISO"       unused   input  active-high 
    line  18:   "SPI0_SCK"       unused   input  active-high 
    line  19:   "SPI0_CS0"       unused   input  active-high 
    line  20:   "SPI0_CS1"       unused   input  active-high 
    line  38:     "GPIO13"       unused   input  active-high 
    line  50:  "UART1_RTS"       unused   input  active-high 
    line  51:  "UART1_CTS"       unused   input  active-high 
    line  76:    "I2S0_FS"       unused   input  active-high 
    line  77:   "I2S0_DIN"       unused   input  active-high 
    line  78:  "I2S0_DOUT"       unused   input  active-high 
    line  79:  "I2S0_SCLK"       unused   input  active-high 
    line 149:     "GPIO01"       unused   input  active-high 
    line 168:     "GPIO07"       unused   input  active-high 
    line 194:     "GPIO12"       unused   input  active-high 
    line 200:     "GPIO11"       unused   input  active-high 
    line 216:     "GPIO09"       unused   input  active-high 
    line 232:   "SPI1_CS1"       unused   input  active-high 
```
## PWM Interface

Sysfs (seems) to be the main interface to control the PWM chips on the hardware. You can see the chips available and how many channels are available with commands below. From this you can then use the C++ class I made to automatically manage it. Note: You should only have one instance of the class per chip, therefore it shouldn't be copied or moved, which is why I have removed those. Its possible you may need to change the pinmux of the device if the PWM channel is used for something else (for example the Jetson Nano Chip 0 Channel 1 is used for something else and you need to change the pinmux and recompile the device tree and linux, but like, good luck with that).

```bash
ls /sys/class/pwm # See what chips are available
cat /sys/class/pwm/pwmchip0/npwm # See how many channels on a chip
```

For a Jetson Nano, the PWM modules are already added to the device tree. For a Raspberry Pi with Ubuntu20.04, you need to add the following lines below to your config.txt (just like when you added start_x=1 etc for using the camera).

```
dtoverlay=pwm-2chan,pin=12,func=4,pin2=13,func2=4
```

# Compilation Instructions

First run the install requirements install script. If you are using a jetson nano or some OS which by default uses an older version of libpiod that is missing c++ bindings, you need to compile install a later version of [libgpoid](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git) from source, following their instructions on how to compile and install (you may also need to add "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" to your ~/.bashrc and run "sudo ldconfig").

Once the setup things are done to comple your program, run the following commands below. These make a new directory for your compiler to build your program, and build the program itself.
```bash
mkdir build && cd build
cmake ..
make -j7
```

To run your compiled program replace the text in quotes. The executable name will be the same one defined in CMakeLists.
```bash
./"your_executable_name"
```
