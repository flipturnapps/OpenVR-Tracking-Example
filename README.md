#  What is this?
This project is a rapid and simple example of how to start a background process for listening to HTC Vive Lighthouse tracking data.  The project interfaces with [Valve's OpenVR SDK](https://github.com/ValveSoftware/openvr).  It was developed cross platform, so the project compiles on both Windows and Linux systems.  

##  How do I run it?
The project needs to be built from source.  Follow the step by step guide below to compile the project.

##  How do I compile it?

Make sure g++ and python is installed. If using [cygwin](https://www.cygwin.com/) for Windows (recommended), these packages may need to be installed from the cygwin installer.  

Have [openvr](https://github.com/ValveSoftware/openvr) cloned somewhere convenient

```shell

git clone https://github.com/flipturnapps/OpenVR-Tracking-Example.git ViveTrack
cd ViveTrack
python build.py ~/path/to/openvr/
bash build/compile.sh

# Start up SteamVR and connect the Vive

./build/track

```



##  Troubleshooting:

*Unable to init VR runtime: Hmd Not Found (108)*

Solution: Attach the HMD to the computer


*Unable to init VR runtime: Not starting vrserver for background app (121)*

Solution: Start Steam VR

*Unable to init VR runtime: vrclient Shared Lib Not Found (102)*

Although this project compiles on Linux, I have been getting this error on every time trying to test it on Linux.  My SteamVR doesn't work on my Linux installation, so it is no suprise that this example doesn't work either.

