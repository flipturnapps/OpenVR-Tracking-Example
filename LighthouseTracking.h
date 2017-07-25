
#ifndef LIGHTHOUSETRACKING_H_
#define LIGHTHOUSETRACKING_H_

#include <headers/openvr.h>
#include <stdio.h>
#include <iostream>
#include "cylinder.h"
#include "cpTime.h"

using namespace vr;

struct _InitFlags
{
		bool printCoords = true;
		bool printAnalog = true;
		bool printEvents = true;
		bool printSetIds = true;
		bool printBEvents = true;
		bool printTrack = true;
};
typedef struct _InitFlags InitFlags;

class LighthouseTracking 
{

private:
	//The pointer to the VR system for VR function calls. Initialized in the constructor.
	IVRSystem* vr_pointer = NULL;

	//Returns xyz coordinates from the matrix that is returned by the VR system functions
	//  see the HMDCoords() and ControllerCoords() methods for usage
	HmdVector3_t GetPosition(HmdMatrix34_t matrix);

	HmdQuaternion_t GetRotation(vr::HmdMatrix34_t matrix);

	InitFlags flags;

public:

	//Destructor
	~LighthouseTracking();

	//Initializes the VR system and the array of cylinders
	LighthouseTracking(InitFlags f);

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

	/* For each controller:
	 * Gets controller state and pose from the VR system
	 * Prints controller coords
	 * Gets/prints button states (analog data) from the conrollers
	 * Rumbles the controllers based on the current mode/inputs
	*/
	void ControllerCoords();
	
	/* CURRENTLY NOT BEING CALLED BY ANY FUNCTION
	 * If called every frame before ControllerCoords, should result in smoother 
	     contoller reconnection after leaving room bounds
	*/
	void setHands();

	void TrackerCoords();

	char* getEnglishTrackingResultForPose(TrackedDevicePose_t pose);
	char* getEnglishPoseValidity(TrackedDevicePose_t pose);
	char* getPoseXYZString(TrackedDevicePose_t pose, int hand);

	struct _ControllerData
	{
		//Fields to be initialzed by iterateAssignIds() and setHands()
   		int deviceId = -1;   // Device ID according to the SteamVR system
    	int hand = -1;       // 0=invalid 1=left 2=right
    	int idtrigger = -1;  // Trigger axis id
    	int idpad = -1;      // Touchpad axis id

    	//Analog button data to be set in ContollerCoods()
    	float padX;
    	float padY;
    	float trigVal;
 		
 		//Position set in ControllerCoords()
    	HmdVector3_t pos;

    	bool isValid;
	};
	typedef struct _ControllerData ControllerData;

	struct _TrackerData
	{
		int deviceId = -1;   // Device ID according to the SteamVR system
    	HmdVector3_t pos;
    	bool isValid;
	};
	typedef struct _TrackerData TrackerData;

	//An array of ControllerData structs
	ControllerData controllers[2];
	TrackerData* trackers;

	//Number of minutes that have elapsed as set in ParseTrackingFrame() 
	//  used for deciding whether or not to run iterateAssignIds()
	long minuteCount = 0;

	int hmdDeviceId = -1;
	
	//Initialized in the constuctor as an array of Cylinder object pointers
	Cylinder** cylinders;

	//If not in drawing mode, then in sensing mode
	//  gets set in dealWithButtonEvent() and tested in ControllerCoords()
	bool inDrawingMode = true;

	//The index of the array of cylinders that is currently being edited
	int cylinderIndex = 0;

	//If true, ControllerCoords() will set rumbleMsOffset to now to trigger rumble if in drawingMode
	bool doRumbleNow = false;
	unsigned long rumbleMsOffset = 0;

	//The number of ms when the grip button was released
	unsigned long gripMillis;
	
	unsigned const int MAX_CYLINDERS = 10;

	char* coordsBuf;
	char* trackBuf;

};

#endif
