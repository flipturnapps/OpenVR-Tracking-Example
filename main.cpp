#include <stdio.h>
#include "LighthouseTracking.h"
#include "cpTime.h"
#include <cstring>
#include <strings.h>

int main( int argc, // Number of strings in array argv  
 char *argv[],      // Array of command-line argument strings  
 char **envp )      // Array of environment variable strings  
{
	InitFlags flags;
	bool isHelp = false;
	bool invert = false;
	bool hasHadFlag = false;
	for (int x = 0; x < argc; x++)
	{
		char* currString = argv[x];
		int len = strlen(currString);
		bool isFlag = len > 1 && currString[0] == '-' && '-' != currString[1];

		if(!hasHadFlag && isFlag)
		{
			hasHadFlag = true;
			invert = !invert;
		}

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
			if(currString[y] == 'r')
				flags.printRotation = false;
			if(currString[y] == 'V')
				flags.pipeCoords = true;
			if(currString[y] == 'O')
				invert = !invert;
		}

		if(!isFlag)
		{
			if( strcasecmp("--help",currString) == 0 )
				isHelp = true;
			if( strcasecmp("--coords",currString) == 0 )
				flags.printCoords = false;
			if( strcasecmp("--analog",currString) == 0 )
				flags.printAnalog = false;
			if( strcasecmp("--events",currString) == 0 )
				flags.printEvents = false;
			if( strcasecmp("--ids",currString) == 0 )
				flags.printSetIds = false;
			if( strcasecmp("--bevents",currString) == 0 )
				flags.printBEvents = false;
			if( strcasecmp("--track",currString) == 0 )
				flags.printTrack = false;
			if( strcasecmp("--rot",currString) == 0 )
				flags.printRotation = false;
			if( strcasecmp("--visual",currString) == 0 )
				flags.pipeCoords = true;
			if( strcasecmp("--omit",currString) == 0 )
				invert = !invert;
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
		flags.printRotation = !flags.printRotation;
	}

	if(isHelp)
	{
		printf("\nVive LighthouseTracking Example by Kevin Kellar.\n");
		printf("Command line flags:\n");
		printf("  -h --help    -> Prints this help text. The \"Only Print\" flags can be combined for multiple types to both print.\n");		
		printf("  -a --analog  -> Only print analog button data from the controllers. \n");
		printf("  -b --bEvents -> Only print button event data. \n");		
		printf("  -c --coords  -> Only print HMD/Controller coordinates. \n");		
		printf("  -e --events  -> Only print VR events. \n");
		printf("  -i --ids     -> Only print the output from initAssignIds() as the devices are given ids. \n");
		printf("  -r --rot     -> Only print the rotation of devices. \n");
		printf("  -t --track   -> Only print the tracking state of devices. \n");
		printf("  -O --omit    -> Omits only the specified output types (a,b,c,e,i,r,t) rather than including only the specified types.  Useful for hiding only a few types of output. \n");
		printf("  -V --visual  -> Streamlines output (coordinates) to be more easily parsed by a visual program. \n");
		return EXIT_SUCCESS;
	}

	if(flags.pipeCoords)
	{
		flags.printCoords = false;
		flags.printAnalog = false;
		flags.printEvents = false;
		flags.printSetIds = false;
		flags.printBEvents = false;
		flags.printTrack = false;
		flags.printRotation = false;
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

