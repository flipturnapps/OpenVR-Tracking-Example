#include "cpTime.h"
#include <stdio.h>

#if defined __linux
	#include <sys/time.h>
	#include <time.h>
	#include <unistd.h>
	bool clockDef = false;
	struct timespec start;
#elif defined _WIN32 || defined __CYGWIN__
	#include <windows.h>

#else     
	#error Platform not supported
#endif

long cpMillis()
{
	#if defined __linux
		if(!clockDef)
		{
			clock_gettime(CLOCK_MONOTONIC_RAW, &start);
			clockDef = true;
		}
		struct timespec  end;
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);

		return ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000)/1000;

	#elif defined _WIN32 || defined __CYGWIN__
		SYSTEMTIME time;
		GetSystemTime(&time);
		return (time.wSecond * 1000) + time.wMilliseconds;

	#else     
		#error Platform not supported
	#endif	
}

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


