#ifndef FILEMATRIX_H_

#define FILEMATRIX_H_
#include <stdio.h>

extern FILE*** fileArrays;
extern bool hasFilesInit;

void init();
void openAll();
void closeAll();
void forEach( void (*func) (FILE*,int o, int i) );
void open(FILE* file, int o, int i);
void close(FILE* file, int o, int i);

#endif