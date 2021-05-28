# ECE3091 Resources

ECE3091 is a design unit at Monash University, where students are required to design, build and program a mobile robot to solve some task. The robot is built around a differential drive platform, Raspberry Pi and camera module, but students are allowed to use whatever processor or custom accessories they like. 3D printed accessories and custom manipulators are encouraged. This repository has selection of code resources for mobile robot navigation and control, and a crash course in object detection. 

* [Robot navigation and control](https://github.com/mgb45/ECE3091_resources/blob/main/Robot_navigation_and_control.ipynb)
* [Simple blob (colour/shape) detection strategy](https://github.com/mgb45/ECE3091_resources/blob/main/Image_processing_detection_strategy.ipynb)
* [Simple CNN (2D regression) detection strategy](https://github.com/mgb45/ECE3091_resources/blob/main/Machine_learning_detection_strategy.ipynb)


# Setting up your raspberry pi to run these notebooks

I recommend you run these notebooks on an alternative machine and not directly on your raspberry pi, as this will be quite slow. However, if you would like to configure your raspberry pi to use jupyter follow the steps below.

## Initial Setup

I'll assume you have an operating system with a terminal, and that can ssh. On windows, check out the [windows subsystem for linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10), and [putty](https://www.putty.org/).

I [installed Ubuntu 20.04 server](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#1-overview) on my raspberry pi, because I wanted a 64 bit operating system for PyTorch (see below).

Let's log into our Ubuntu raspberry pi using ssh - the default user and password is ubuntu
```
ssh ubuntu@ip_address
```
Once you're in, it's time to install some useful software:

```
sudo apt-get update
sudo apt-get install python3-scipy jupyter libatlas-base-dev avahi-daemon tmux htop vim
```

Since we've installed the [avahi-daemon](http://avahi.org/), it's a good idea to change the hostname to something unique, eg. `ECE3091_<GROUPNO>`. Use the command 
```
hostnamectl set-hostname ECE3091_<GROUPNO>
```
to do this. You may need to reboot `sudo reboot` for this to take effect.

Now, assuming you're on the same network, logging in to the pi remotely will be a lot easier going forward, just using:
```
ssh pi@ECE3091_<GROUPNO>.local
```

Ok, lets continue to install some packages on the pi.
``` 
pip3 install --upgrade pip
pip3 install --user jupyter opencv-python
```

Next, we'll clone this repository
```
git clone https://github.com/mgb45/ECE3091_resources/
```

# Running the notebooks

Now we'll run jupyter notebook in a headless fashion
```
jupyter notebook --no-browser --port 8888 --ip 0.0.0.0
```

You can now access the notebook running on the raspberry pi using any other computer on the network, by simply typing
```
http://raspberry_pi_ip:8888/
```
into a browser address bar, where raspberry_pi_ip is the ip address of your raspberry pi. Eg. mine is 192.168.20.14.

You can find out your raspberry pi ip address by typing `ifconfig` in a raspberry pi terminal, or going to your router settings pages and looking at the addresses of connected devices.

Use `ctrl-c` in the terminal to kill a running process (eg. if you want to close the notebook).

### Running PyTorch on your raspberry pi

I found this [guide](https://qengineering.eu/install-pytorch-on-raspberry-pi-4.html) very helpful. However, the raspberry pi is really not suited to deep learning (Consider the [Nvidia Jetson](https://developer.nvidia.com/buy-jetson) if you want a better embedded computer for this). You can run models, but don't try to train on the pi.


