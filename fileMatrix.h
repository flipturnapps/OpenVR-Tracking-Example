#ifndef FILEMATRIX_H_

#define FILEMATRIX_H_
#include <stdio.h>

extern FILE*** fileArrays;
extern bool hasFilesInit;

void filesInit();
void openAll();
void closeAll();
void forEach( void (*func) (FILE*,int o, int i) );
void filesOpen(FILE* file, int o, int i);
void filesClose(FILE* file, int o, int i);

#endif