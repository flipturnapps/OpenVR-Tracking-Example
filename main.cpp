//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016
//

#include <stdio.h>
#include "LighthouseTracking.h"

#if defined __linux
#include <unistd.h>

#elif defined _WIN32 || defined __CYGWIN__
#include <windows.h>

#else     
#error Platform not supported
#endif

void cpSleep(int sleepMs)
{
#if defined __linux
    usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)

#elif defined _WIN32 || defined __CYGWIN__
    Sleep(sleepMs);

#else     
#error Platform not supported
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

