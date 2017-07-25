#include "fileMatrix.h"

FILE*** fileArrays;
bool hasFilesInit = false;

void init()
{
	if(hasFilesInit)
		closeAll();

	fileArrays = new FILE**[2];
	fileArrays[0] = new FILE*[4];
	fileArrays[1] = new FILE*[4];

	openAll();

	hasFilesInit = true;
}

void openAll()
{
	void (*func)(FILE*,int o, int i);
	func = open;
	forEach(func);
}

void closeAll()
{
	void (*func)(FILE*,int o, int i);
	func = open;
	forEach(func);
}

void forEach( void (*func) (FILE*,int o, int i) )
{
	for(int o = 0; o < 2; o++)
	{
		for (int i = 0; i < 4; i++)
		{
			FILE* file = fileArrays[o][i];
			func(file,o,i);
		}
	}
}

void open(FILE* file, int o, int i)
{
	char* name = new char[20];
	if(o == 0)
		sprintf(name,"left-");
	if(o == 1)
		sprintf(name,"right-");
	if(i == 0)
		sprintf(name,"%st",name);
	if(i == 1)
		sprintf(name,"%sx",name);
	if(i == 2)
		sprintf(name,"%sy",name);
	if(i == 3)
		sprintf(name,"%sz",name);
	file = fopen(name,"w");	
}

void close(FILE* file, int o, int i)
{
	fclose(file);
}