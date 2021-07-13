See what's available with the command below

```bash
v4l2-ctl --list-devices
```

See what you can change with, for example you may want to reduce the exposure time to reduce motion blur

```bash
v4l2-ctl -d /dev/video0 --list-ctrls-menus
```

You can then change the properties with command line args

```bash
v4l2-ctl --set-ctrl=auto_exposure_bias=6
```

Capture test video efficiently with the command below, not any resolution or frame rate is supported, you need to check.

```bash
ffmpeg -f v4l2 -framerate 25 -video_size 640x480 -i /dev/video0 output.mkv
```
