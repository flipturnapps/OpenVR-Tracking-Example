// LIGHTHOUSETRACKING.h
#ifndef LIGHTHOUSETRACKING_H_
#define LIGHTHOUSETRACKING_H_

#include <headers/openvr.h>
#include <stdio.h>
#include <iostream>

using namespace vr;


class LighthouseTracking {
private:
	IVRSystem *vr_pointer = NULL;
	HmdVector3_t GetPosition(HmdMatrix34_t matrix);
	bool bWaitForEventsBeforeParsing = false;

public:
	~LighthouseTracking();
	LighthouseTracking();
	bool RunProcedure(bool bWaitForEvents);
	bool ProcessVREvent(const VREvent_t & event);
	void ParseTrackingFrame();
};

#endif
