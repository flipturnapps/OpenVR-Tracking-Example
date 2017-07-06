//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016
//

#include <stdio.h>
#include "LighthouseTracking.h"

#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif

void cpSleep(int sleepMs)
{
#ifdef LINUX
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef WINDOWS
    Sleep(sleepMs);
#endif
}

int main()
{
	// If false we'll parse tracking data continously, if true we parse when an openvr event fires
	bool bAcquireTrackingDataByWaitingForVREvents = false;

	// Create a new LighthouseTracking instance and parse as needed
	LighthouseTracking *lighthouseTracking = new LighthouseTracking();
	if (lighthouseTracking) {
		char buf[1024];
		printf(buf, sizeof(buf), "Press 'q' to quit. Starting capture of tracking data...\n");
		printf(buf);
		cpSleep(2000);

		while (lighthouseTracking->RunProcedure(bAcquireTrackingDataByWaitingForVREvents)) {

			
			cpSleep(1);
		}

		delete lighthouseTracking;
	}
	return EXIT_SUCCESS;
}

