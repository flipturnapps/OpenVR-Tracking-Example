#include <stdio.h>
#include "LighthouseTracking.h"
#include "cpTime.h"


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

