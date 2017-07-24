#include <stdio.h>
#include "LighthouseTracking.h"
#include "cpTime.h"
#include <string.h>

int main( int argc, // Number of strings in array argv  
 char *argv[],      // Array of command-line argument strings  
 char **envp )      // Array of environment variable strings  
{
	InitFlags flags;
	bool isHelp = false;
	bool invert = false;
	for (int x = 0; x < argc; x++)
	{
		char* currString = argv[x];
		int len = strlen(currString);
		bool isFlag = len > 1 && currString[0] == '-' && '-' != currString[1];

		if (isFlag)
		for(int y = 1; y < len; y++)
		{
			if(currString[y] == 'h')
				isHelp = true;
			if(currString[y] == 'c')
				flags.printCoords = false;
			if(currString[y] == 'a')
				flags.printAnalog = false;
			if(currString[y] == 'e')
				flags.printEvents = false;
			if(currString[y] == 'i')
				flags.printSetIds = false;
			if(currString[y] == 'b')
				flags.printBEvents = false;
			if(currString[y] == 't')
				flags.printTrack = false;
			if(currString[y] == 'I')
				invert = true;
		}

		if(!isFlag)
		{
			if( strcasecmp("--help",currString) == 0 )
				isHelp = true;
			if( strcasecmp("--nocoords",currString) == 0 )
				flags.printCoords = false;
			if( strcasecmp("--noanalog",currString) == 0 )
				flags.printAnalog = false;
			if( strcasecmp("--noevents",currString) == 0 )
				flags.printEvents = false;
			if( strcasecmp("--noids",currString) == 0 )
				flags.printSetIds = false;
			if( strcasecmp("--nobevents",currString) == 0 )
				flags.printBEvents = false;
			if( strcasecmp("--notrack",currString) == 0 )
				flags.printTrack = false;
			if( strcasecmp("--invert",currString) == 0 )
				invert = true;
		}
	}

	if(invert)
	{
		flags.printCoords = !flags.printCoords;
		flags.printAnalog = !flags.printAnalog;
		flags.printEvents = !flags.printEvents;
		flags.printSetIds = !flags.printSetIds;
		flags.printBEvents = !flags.printBEvents;
		flags.printTrack = !flags.printTrack;
	}

	if(isHelp)
	{
		printf("Vive LighthouseTracking Example by Kevin Kellar.\n");
		printf("Command line arguments:\n");
		printf("  -h --help      -> Prints this help text. \n");		
		printf("  -a --noAnalog  -> Do not print analog button data from the controllers. \n");
		printf("  -b --noBevents -> Do not print button event data. \n");		
		printf("  -c --noCoords  -> Do not print HMD/Controller coordinates. \n");		
		printf("  -e --noEvents  -> Do not print VR events as they happen. \n");
		printf("  -i --noIds     -> Do not print the output from initAssignIds() as the devices are given ids. \n");
		printf("  -t --noTrack   -> Do not print the tracking state of devices. \n");
		printf("  -I --invert    -> Inverts the noprint (a,b,c,e,i) flags.  Useful for specifying the few types of output you want. \n");
		return EXIT_SUCCESS;
	}
	
	// Create a new LighthouseTracking instance and parse as needed
	LighthouseTracking* lighthouseTracking = new LighthouseTracking(flags);
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

