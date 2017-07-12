// LIGHTHOUSETRACKING.h
#ifndef LIGHTHOUSETRACKING_H_
#define LIGHTHOUSETRACKING_H_

#include <headers/openvr.h>
#include <stdio.h>
#include <iostream>

class LighthouseTracking {
private:
	vr::IVRSystem *vr_pointer = NULL;
	vr::HmdVector3_t GetPosition(vr::HmdMatrix34_t matrix);
	bool bWaitForEventsBeforeParsing = false;

public:
	~LighthouseTracking();
	LighthouseTracking();
	bool RunProcedure(bool bWaitForEvents);
	bool ProcessVREvent(const vr::VREvent_t & event);
	void ParseTrackingFrame();
};

#endif
