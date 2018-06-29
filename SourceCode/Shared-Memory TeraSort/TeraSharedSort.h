#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include<pthread.h>
#include <time.h>
#include<math.h>
#include"memoryManage.c"
#include"mergeFiles.c"
#include"mergeSort.c"

int MemFree;  // Total free memory
int NumPTH;   // Total pthreads

long l=0;
//      FILE *filePointer;  // file pointer shared amoung different threads

char inputFilename[100] = "input.txt";

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
int Csort = 0;
clock_t start, start1,start2,mergeStart, end,end1,SortEnd;
void* mergeSortFile(void *param );

void SplitFile(long int TotalFileSize,long int memfree);
