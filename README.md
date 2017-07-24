#  What is this?

This project is a rapid and simple example of how to start a background process for listening to HTC Vive Lighthouse tracking data.  The project interfaces with [Valve's OpenVR SDK](https://github.com/ValveSoftware/openvr).  It was developed cross platform, so the project should compile on both Windows and Linux systems.  If you find that this is not the case, open an issue or contact the developer.

##  How do I run it?
The project needs to be built from source.  Follow the step by step guide below to compile the project.

##  How do I compile it?

Make sure **g++** and **python** are installed. If using [cygwin](https://www.cygwin.com/) for Windows (recommended), these packages may need to be installed from the cygwin installer.  

Have [openvr](https://github.com/ValveSoftware/openvr) cloned somewhere convenient

This in short, the following script should properly build and run the project: 

```shell

git clone https://github.com/flipturnapps/OpenVR-Tracking-Example.git ViveTrack
cd ViveTrack
python build.py ~/path/to/openvr/
bash build/compile.sh

# Start up SteamVR and connect the Vive

./build/track

```
More information about the compile process and screenshots of what you should see throughout the process can be found on the [Compile Guide](https://github.com/flipturnapps/OpenVR-Tracking-Example/wiki/CompileGuide).

## How do I play it?

This branch of the Vive Tracking Example tracks the locations of controllers and the HMD and prints the coordinates, event data, and button data to stdout.  However, to further demonstrate the tracking potential of the Vive system, the bulk of the code on this branch is for a tool where the user draws around obstacles in the room using the Vive controllers so that, when in sensing mode, the controllers can rumble when nearby the obstacles as a "warning".

### Defining a cylinder around a real-world obstacle

As of now, the only shapes that can be used for collision testing are cylinders with a height in the direction of the y-axis.  Here are the steps within the program to define an obstacle (cylinder):

1. Enter DrawingMode. You can toggle between DrawingMode and SensingMode with the ApplicationMenu button above the trackpad.  You have entered DrawingMode when the controller rumbles breifly after pressing the ApplicationMenu button.
2. The next step defines a 2D circle in the xz plane.  Hold down the trigger with the tip of the controller touching one edge of your obstacle and release while touching the opposide edge of the obstacle.  The distance between the two points will be the cirle's diameter and the midpoint betwen the two will become the center of the circle.

<img src="http://i.imgur.com/5mWe6Ba.png" width="300">

3. By default, the y-axis bounds of the cylinder extend forever in both directions. 

<img src="http://i.imgur.com/BAMtXej.png" width="300">

4. Define vertical boundaries by holding down the grip button with the controller level to the top of the obstacle and releasing level with the bottom of the obstacle.

<img src="http://i.imgur.com/SW4jyyI.png" width="300">

5. Switch to SensingMode by pressing the ApplicationMenu button, and test to see if the controllers will rumble if nearby the obstacle.

### Extending vertical bounds infinitely in one direction

Often, the obstacles you might want to test with are on the floor and don't have any empty space under them.  To define the obstacle with infinite depth going downwards so that you don't have to rub your Vive controllers on the ground to properly define the obstacle, use the following control: 

1. Follow steps 1 & 2 above.
2. Hold down the grip while the controller is level with the top of the obstacle.
3. Lower the controller an arbitrary distance below where the grip was originally heald.
4. Release the grip and rapidly press and release the grip again to extend the boundary forever in that direction.

### Drawing multiple cylinders

With the current implementation, up to ten different cylinders can be drawn around obstacles.  While in DrawingMode, the controller will occasionally rumble the current index of cylinder that is being edited (starting with one).  To move to the next cylinder, press the right side of the touchpad.  To edit a previous cylinder, press the left side.

When moving to different cylinder, the controllers should vibrate the current index that has been selected.

### Controls Summary

All of the following controls (except toggling modes with the ApplicationMenu button) only work in DrawingMode.

| Button  |   Function 
|----------|:-------------|
| Application Menu | Toggle from DrawingMode to SensingMode and back  | 
| Trigger          | Hold to define or reset the circle that is a cross-section of the cylinder in the xz plane |
| Grip             | Hold to define the vertical bounds of the current cylinder |
| Trackpad (left)  | Switch work to the previous cylinder |
| Trackpad (right) | Switch work to the next cylinder |
| Trackpad (up)    | Press to vibrate the index of the cylinder currently being worked on |
| Trackpad (down)  | Reset/delete the cylinder at the current index |


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

## Features in Development

- [ ] All current source code properly commented
- [ ] Allow for controller states to be tested/printed in English like original project
- [ ] Use command line arguments to decide what information to print
- [ ] Make some sort of index to easily find answers to how to do simple VR library calls
