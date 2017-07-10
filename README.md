#  What is this?
This project is a rapid and simple example of how to start a background process for listening to HTC Vive Lighthouse tracking data.  The project interfaces with [Valve's OpenVR SDK](https://github.com/ValveSoftware/openvr).  It was developed cross platform, so the project compiles on both Windows and Linux systems.  

##  How do I run it?
The project needs to be built from source.  Follow the step by step guide below to compile the project.

##  How do I compile it?

* Make sure g++ and python is installed.  
  * If using [cygwin](https://www.cygwin.com/) for Windows (recommended), these packages may need to be installed from the cygwin installer.
* Have [openvr](https://github.com/ValveSoftware/openvr) cloned somewhere convenient

1. Clone this repository and cd to the cloned repo
2. Run the python script with the path to the openvr sdk as a command line argument ```python build.py /path/to/openvr/clone```
3. Run the compile command with ```bash /build/compile.sh```
4. The executable can be found at ```/build/track```


##  How do I use it?
1. Start up Steam VR.
2. Run the exetuable: ```./build/track```



##  Troubleshooting:

*Unable to init VR runtime: Hmd Not Found (108)*

Solution: Attach the HMD to the computer


*Unable to init VR runtime: Not starting vrserver for background app (121)*

Solution: Start Steam VR

