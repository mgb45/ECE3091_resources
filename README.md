# ECE3091 Resources

ECE3091 is a design unit at Monash University, where students are required to design, build and program a mobile robot to solve some task. This repository has selection of code resources for mobile robot navigation and control, and a crash course in object detection. 

* [Robot navigation and control](https://github.com/mgb45/ECE3091_resources/blob/main/Robot_navigation_and_control.ipynb)
* [Simple blob (colour/shape) detection strategy](https://github.com/mgb45/ECE3091_resources/blob/main/Image_processing_detection_strategy.ipynb)
* [Simple CNN (2D regression) detection strategy](https://github.com/mgb45/ECE3091_resources/blob/main/Machine_learning_detection_strategy.ipynb)


# Setting up your raspberry pi to run these notebooks

I recommend you run these notebooks on an alternative machine and not directly on your raspberry pi, as this will be quite slow. However, if you would like to configure your raspberry pi to use jupyter follow the steps below.

Let's log into our raspberry pi using ssh, if you haven't changed the raspberry pi hostname, this should be:
```
ssh raspberrypi.local
```

### Then, we'll install some packages
```sudo apt-get update
sudo apt-get install python3-scipy jupyter libatlas-base-dev
pip3 install --upgrade pip
pip3 install --user jupyter opencv-python
```

### Next, we'll clone this repository
```
git clone https://github.com/mgb45/ECE3091_resources/
```

### Now we'll run jupyter notebook in a headless fashion
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


