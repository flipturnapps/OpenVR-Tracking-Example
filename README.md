#  What is this?
This project is a rapid and simple example of how to start a background process for listening to HTC Vive Lighthouse tracking data.  The project interfaces with [Valve's OpenVR SDK](https://github.com/ValveSoftware/openvr).  It was developed cross platform, so the project compiles on both Windows and Linux systems.  

##  How do I run it?
The project needs to be built from source.  Follow the step by step guide below to compile the project.

##  How do I compile it?

Make sure **g++** and **python** are installed. If using [cygwin](https://www.cygwin.com/) for Windows (recommended), these packages may need to be installed from the cygwin installer.  

Have [openvr](https://github.com/ValveSoftware/openvr) cloned somewhere convenient

This example code shows how to build/run the project: 

```shell

git clone https://github.com/flipturnapps/OpenVR-Tracking-Example.git ViveTrack
cd ViveTrack
python build.py ~/path/to/openvr/
bash build/compile.sh

# Start up SteamVR and connect the Vive

./build/track

```

These screenshots show the terminal at the different stages of building and running the example:

| Linux  |      Windows      
|----------|:-------------:|
| ![Py script on Linux](http://i.imgur.com/ny2F7Tr.png) |  ![Python script on Windows](http://i.imgur.com/JJ5JQBN.png) |
| ![Linux: running compilation command](http://i.imgur.com/LJLZnbc.png) | ![Windows: running complation command](http://i.imgur.com/ywUUtpU.png) |
| ![Linux: compiling succeeds](http://i.imgur.com/AwVaQDs.png) | ![Windows: compiling succeeds](http://i.imgur.com/QS3EmGE.png) |
| ![Errors on linux](http://i.imgur.com/NNOviFB.png) | ![Runnning the example on windows](http://i.imgur.com/QcLUkPC.png)|



##  Troubleshooting:

### *Unable to init VR runtime: Hmd Not Found (108)*

Solution: Attach the HMD to the computer


### *Unable to init VR runtime: Not starting vrserver for background app (121)*

Solution: Start Steam VR

### *Unable to init VR runtime: vrclient Shared Lib Not Found (102)*

I fixed this issue using [this thread](https://github.com/cmbruns/pyopenvr/issues/30) from the [pyopenvr](https://github.com/cmbruns/pyopenvr) repository.  According to this thread, the *libudev.so.1* object needs to be linked to *libudev.so.0* on Linux, using ```sudo ln -s /lib/x86_64-linux-gnu/libudev.so.1 /lib/x86_64-linux-gnu/libudev.so.0```.  Creating this link fixed the above problem for me, but according to an [askubuntu thread](https://askubuntu.com/questions/604331/libudev-so-1-not-found-when-trying-to-run-spotify), this fix can break Unity.  I also went ahead and followed the other fixes in the pyopenvr thread.  

I set my LD_LIBRARY_PATH to
>/home/<user>/.local/share/Steam/steamapps/common/SteamVR/bin/linux64:/home/<user>/.local/share/Steam/steamapps/common/tools/bin/linux64:.

And I installed the following packages on aptitude:
* libudev-dev
* libvulkan-dev
* libsdl2-dev
* libglfw3-dev


