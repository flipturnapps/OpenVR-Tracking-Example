//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016
//

#include <stdio.h>
#include "LighthouseTracking.h"

int main()
{
	// If false we'll parse tracking data continously, if true we parse when an openvr event fires
	bool bAcquireTrackingDataByWaitingForVREvents = false;

	// Create a new LighthouseTracking instance and parse as needed
	LighthouseTracking *lighthouseTracking = new LighthouseTracking();
	if (lighthouseTracking) {
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Press 'q' to quit. Starting capture of tracking data...\n");
		printf_s(buf);
		Sleep(2000);

		while (lighthouseTracking->RunProcedure(bAcquireTrackingDataByWaitingForVREvents)) {

			
			Sleep(1);
		}

		delete lighthouseTracking;
	}
	return EXIT_SUCCESS;
}

