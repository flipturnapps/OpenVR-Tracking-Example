#include <stdio.h>
#include "LighthouseTracking.h"
#include "cpTime.h"
#include <string.h>

int main( int argc, // Number of strings in array argv  
 char *argv[],      // Array of command-line argument strings  
 char **envp )      // Array of environment variable strings  
{
	for (int i = 0; i < argc; i++)
	{
		printf("--ARGECHO #%d: %s\n",i,argv[i]);
	}
	InitFlags flags;
	for (int x = 0; x < argc; x++)
	{
		char* currString = argv[x];
		int len = strlen(currString);
		bool isFlag = len > 1 && currString[0] == '-' && '-' != currString[1];
		printf("--STRLEN %s is %d long. flag=%s\n",currString,len,isFlag ? "true" : "false");

		if (isFlag)
		for(int y = 1; y < len; y++)
		{
			if(currString[y] == 'h')
				printf("help\n");
			if(currString[y] == 'c')
				printf("coords\n");
			if(currString[y] == 's')
				printf("states\n");
		}

		if(!isFlag)
		{
			if( strcasecmp("--help",currString) == 0 )
				printf("bighelp\n");
		}
	}


	return EXIT_SUCCESS;


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

