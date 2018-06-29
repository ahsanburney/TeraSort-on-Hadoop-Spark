#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include <mpi.h>
#include<math.h>
#define MASTER 0

#include <time.h>

#include"memoryManage.c"
#include"mergeMPIFiles.c"
#include"mergeMPISort.c"

int MemFree;  // Total free memory
int NumProc;   // Total pthreads

long l=0;
//      FILE *filePointer;  // file pointer shared amoung different threads

char inputFilename[100] = "input.txt";

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
int Csort = 0;
clock_t start, start2,mergeStart, end,SortEnd;
void mergeMPISortFile(int param );
void SplitFile(long int TotalFileSize,long int memfree);

