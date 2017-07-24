
#ifndef LIGHTHOUSETRACKING_H_
#define LIGHTHOUSETRACKING_H_

#include <headers/openvr.h>
#include <stdio.h>
#include <iostream>
#include "cylinder.h"
#include "cpTime.h"

using namespace vr;

class LighthouseTracking 
{

private:
	//The pointer to the VR system for VR function calls. Initialized in the constructor.
	IVRSystem* vr_pointer = NULL;

	//Returns xyz coordinates from the matrix that is returned by the VR system functions
	//  see the HMDCoords() and ControllerCoords() methods for usage
	HmdVector3_t GetPosition(HmdMatrix34_t matrix);

public:

	//Destructor
	~LighthouseTracking();

	//Initializes the VR system and the array of cylinders
	LighthouseTracking();

	/* Is called in a loop from the main methods in main.cpp
	 * Returns false if the VR system has stopped.
	 * First polls a VR event and then runs ProcessVREvent
	 * Returns false (to exit the program) if ProcessVREvent returns false.
	 * If the system hasn't shut down, calls ParseTrackingFrame()
	*/  
	bool RunProcedure();

	/* Takes a VREvent pointer when called by RunProcedure()
	 * Switches through the common VREvents
	 * If it is a button event (#200-203) then calls dealWithButtonEvent 
	 * Returns false if the event should cause the application to quit (like SteamVR quitting)
	*/
	bool ProcessVREvent(const VREvent_t & event);

	/* First prints the button event data to the terminal
	 * Tests to see if it was the ApplicationMenu button that was pressed. If so, switch modes
	 * Tests that it was a grip press or unpress event and stores the y-bounds for the current cylinder if applicable
	 * Tests if it was a trigger press or unpress and stroes the x/z bounds for the current cylinder if applicable
	 * Finally, tests if it was a touchpad press and increments, decrements, 
	     deletes or rumbles the current cylinder depending on where the press was
	*/
	void dealWithButtonEvent(VREvent_t);

	/* First decides whether or not to call iterateAssignIds() see the conditions in .cpp
	 * Then calls HMDCoords() and ControllerCoords()
	*/
	void ParseTrackingFrame();

	/* Iterates across all the possible device ids (I think the number is 16 for the Vive)
	 * For each possible id, tests to see if it is the HMD or a controller
	 * If it is the HMD, assign hmdDeviceId to the HMD's id
	 * If it is a controller, assign one of the controller structs' deviceId to the id
	     then set handedness and the axis ids of the controller.
	*/
	void iterateAssignIds();

	/* One of the best methods to look at if just trying to learn how to print tracking data
	 * In only about five lines, gets the HMD pose from the VR system, converts the pose to xyz 
	     coordinates, and prints this data to the terminal
	*/
	void HMDCoords();

	
	void ControllerCoords();
	
	void setHands();

	struct _ControllerData
	{
   		int deviceId = -1;
    	int hand = -1; 
    	int idtrigger = -1;
    	int idpad = -1;

    	float padX;
    	float padY;
    	float trigVal;

    	HmdVector3_t pos;
	};
	typedef struct _ControllerData ControllerData;

	ControllerData controllers[2];

	long minuteCount = 0; 
	int hmdDeviceId = -1;
	long controllerInitCount = 0;
	
	
	Cylinder** cylinders;

	bool inDrawingMode = true;
	int cylinderIndex = 0;
	bool doRumbleNow = false;
	unsigned long rumbleMsOffset = 0;

	unsigned long gripMillis; //Stores the number of ms elapsed when the grip was released.
	unsigned const int MAX_CYLINDERS = 10;
};

#endif
