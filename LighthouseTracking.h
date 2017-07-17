// LIGHTHOUSETRACKING.h
#ifndef LIGHTHOUSETRACKING_H_
#define LIGHTHOUSETRACKING_H_

#include <headers/openvr.h>
#include <stdio.h>
#include <iostream>
using namespace vr;


class LighthouseTracking 
{
private:
	IVRSystem *vr_pointer = NULL;
	HmdVector3_t GetPosition(HmdMatrix34_t matrix);
	bool bWaitForEventsBeforeParsing = false;


public:
	~LighthouseTracking();
	LighthouseTracking();
	bool RunProcedure();
	bool ProcessVREvent(const VREvent_t & event);
	void ParseTrackingFrame();
	void iterateAssignIds();
	void HMDCoords();
	void ControllerCoords();
	void dealWithButtonEvent(VREvent_t);

	struct _ButtonData
	{
		bool pressed = false;
		bool touched = false;
	};
	typedef struct _ButtonData ButtonData;

	struct _ControllerData
	{
   		int deviceId = -1;
    	int hand = -1; 
    	int idtrigger = -1;
    	int idpad = -1;

    	ButtonData b_AppMenu;
    	ButtonData b_Trigger;
    	ButtonData b_Grip;
    	ButtonData b_Pad;

    	HmdVector3_t pos;
	};
	typedef struct _ControllerData ControllerData;

	ControllerData controllers[2];

	int hmdDeviceId = -1;
	int controllerInitCount = 0;
	int initPassCount = 0;
};

#endif
