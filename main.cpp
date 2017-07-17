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
	// Create a new LighthouseTracking instance and parse as needed
	LighthouseTracking* lighthouseTracking = new LighthouseTracking();
	if (lighthouseTracking) //null check
	{
		cpSleep(2000);
		while (1==1)
		{
			lighthouseTracking->RunProcedure();
			cpSleep(1);
		}
		delete lighthouseTracking;
	}
	return EXIT_SUCCESS;
}

