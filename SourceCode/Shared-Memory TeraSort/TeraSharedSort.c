
/* C program for Merge Sort */
#include"TeraSharedSort.h"
int main(int argc, char **argv)
{

	double cpu_time_used;
	char outputFilename[100] = "output.txt";
	NumPTH = 2;
	int i;
	if(argc<4)
	{	
		printf(" Please Enter given Format : ./run input.txt output.txt 2");
		return 0;
	}
	strcpy(inputFilename,argv[1]);  // first argument inputfile name
	strcpy(outputFilename,argv[2]);  // Second argument outfile name
	NumPTH = atoi( argv[3] );	// third argument number of threads
	
	
	
	printf("\n***\tShare Memory Terasort using Pthread and MergeSort\t ***\n");
	
	MemFree = GetRamInKB(0); // find total free memory
	
	printf("\nInput file name: %s ", inputFilename);
	
	printf("\nOutput file name: %s ",outputFilename);

	printf("\nNumber of threads: %d \n",NumPTH);


	FILE *filePointer;
	

        int blockSize = 100;  // line size in gensort program generate file

        if(!(filePointer = fopen(inputFilename, "r")))  // open input file
        {
                printf("\n mergeSortFile: Input File open failed.\n");
                exit(EXIT_FAILURE);
        }

        fseek(filePointer,0L,SEEK_END);  // calculate total size of file by moving file pointer to end
        
	long int  TotalFileSize=ftell(filePointer); 
	

	printf("\nInput File Size       : %.2f GB ",((double)TotalFileSize)/(1024*1024*1024));
	
	printf("\nTotal Memory(RAM)     : %.2f GB ",((double)GetRamInKB(2))/(1024*1024));
	printf("\nAvailable Memory(RAM) : %.2f GB ",((double)MemFree)/(1024*1024));
	long int memslot = ( MemFree   )/( NumPTH * 2 );
	long int chunks = NumPTH * ceil(((((double)TotalFileSize)/1024)/(memslot*NumPTH)));
	
	fclose(filePointer);
	start = clock();  
	printf("\n ---- FileSize %ld \n",TotalFileSize);	
	SplitFile( TotalFileSize/100,(MemFree*((double)(1024/100))));
	
	double splitTime = ((double) (clock() - start)) / CLOCKS_PER_SEC;
	printf("Completed splitting files ! \n " );  
	printf("\nTotal No. chunks Created Based on Avaliable Memory & Threads  : %d \n\n",(int)chunks);
	
	start1 = clock();  
	pthread_t threads[ NumPTH ]; 
        for (i = 0; i < NumPTH; i++ ){
			
		int *param = malloc( sizeof( int ));
        	if ( param == NULL ) {
                       fprintf( stderr, "main: Couldn't allocate memory for param.\n" );
                        exit( EXIT_FAILURE );
              	}
                *param = i;
                pthread_create( &threads[ i ], NULL, mergeSortFile, param );  // Pthread call function mergeSortFile and sends thread count
        }
        
        for (i = 0; i < NumPTH; i++){
                pthread_join( threads[i], NULL );  // Pthread wait all pthread to complete funcations
                 }
 	end1 = clock();
	char *files[NumPTH];
        
	for (i = 0; i <NumPTH; i++){  //this loop create list of files to sort 
		files[i] =(char *) malloc(100*sizeof(char));
                if ( files[i] == NULL ) {
                        fprintf(stderr, "main: Couldn't allocate memory for files.\n");
                        exit(EXIT_FAILURE);
                 }
		char rmfiles[50];
		sprintf(files[i],"Thread%d.txt",i);
	}
	printf("\n Main Thread : Merging Started for Thread Chunks\n\n");
	mergeAllSortFile(files,NumPTH,outputFilename,0); // call function to merge all chunks 
		
	for( i = 0;i < NumPTH; i++ )
	{
		free( files[i]);
	}
 	end = clock();
        printf("----------------------------------------------------------------------------");
     	printf("\n\t Total Splitting Time for chunks          = %.2lf sec \n  ",splitTime );
     	cpu_time_used = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
     	printf("\n\t Total Sorting Time for chunks            = %.2lf sec \n  ", cpu_time_used);
     	
	cpu_time_used = ((double) (end - end1)) / CLOCKS_PER_SEC;
	printf("\n\t Total Merging Time for Sorted chunks     = %.2lf sec  \n", cpu_time_used);
        
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     	printf("\n\n\t Total Shared Memory Tera Sorting Time    = %.2lf sec  \n", cpu_time_used);
        printf("----------------------------------------------------------------------------\n");
        
 return 0;

}
void SplitFile(long int TotalFileSize,long int memfree)
{
	char infile[100];
	FILE *filePointer,*fP;
	int i;
	if(!(filePointer = fopen(inputFilename, "r")))  // open input file
        {
                printf("\n mergeSortFile: Input File open failed.\n");
                exit(EXIT_FAILURE);
        }
	//printf("\n ---- FileSize %ld -- %ld \n",TotalFileSize,memfree);	
	
	printf("\n\nInitializing Sorting of Chunks\n ");
	if((TotalFileSize/(NumPTH+1))<memfree)
	{
	printf("\n ---- FileSize %ld -- %ld \n",TotalFileSize,memfree);	
		char **buffer = (char **)malloc(TotalFileSize/(NumPTH)*sizeof(char *));
		for( i = 0; i <(TotalFileSize/(NumPTH)) ; i++ )
                {
                      	buffer[i]=(char *)malloc(100 * sizeof(char));
		}
       		for(int k=0;k<NumPTH;k++)
        	{
                	sprintf(infile,"input%d",k);
                	if (!( fP = fopen( infile, "w" ))) {
                      		fprintf(stderr, "mergeSortFile: File opened failed \n");
                        	exit(EXIT_FAILURE);
                	}
                	for( i = 0; i <(TotalFileSize/(NumPTH)) ; i++ )
                	{
                        	fread( buffer[i],100, 1 , filePointer );
                	}
                	for( i = 0; i <(TotalFileSize/(NumPTH)) ; i++ )
                	{
                        	fwrite( buffer[i], 1, 100, fP );
                	}
                	fclose( fP );
        	}
		for( i = 0; i <(TotalFileSize/(NumPTH)) ; i++ )
                {
                      	free(buffer[i]);
		}
		free(buffer);
	}
        else
	{
		char *buffer = (char *)malloc(100*sizeof(char));
                for(int k=0;k<NumPTH;k++)
                {
                        sprintf(infile,"input%d",k);
                        if (!( fP = fopen( infile, "w" ))) {
                                fprintf(stderr, "mergeSortFile: File opened failed \n");
                                exit(EXIT_FAILURE);
                        }
                        for( i = 0; i <(TotalFileSize/(NumPTH)) ; i++ )
                        {
                                fread( buffer,100, 1 , filePointer );
                                fwrite( buffer, 1, 100, fP );
                        }
                        fclose( fP );
                }
        }
	fclose(filePointer);
}

// This function divides files into different chunks and do mergesort on small chunks

void* mergeSortFile(void *param )
{
        int nth = *((int *) param);

	printf("******* Thread %d Started Sorting ***********\n",nth+1);

	//printf(" Thread Count = %d ",nth);
	int i;
	FILE *filePointer;
		
        int blockSize = 100;  // line size in gensort program generate file
     	char infile[100];
        sprintf(infile,"input%d",nth);
        if(!(filePointer = fopen(infile, "r")))  // open input file
     	{
                printf("\n mergeSortFile: Input File open failed.\n");
                exit(EXIT_FAILURE);
    	}
	fseek(filePointer,0L,SEEK_END);  // calculate total size of file by moving file pointer to end
      	
        long int TotalLines=ftell(filePointer)/blockSize;   // calculate total lines in file
        
	long int posStart = (TotalLines/NumPTH)*nth;  ;//Set File pointer Start based on thread count
	
        long int posEnd;  
	
        if(nth == NumPTH-1)                             // if this function called by last thread then rest of lines is alloted to function
		posEnd   = TotalLines;                  // total no of lines 
	else
		posEnd   = (TotalLines/NumPTH)*(nth+1); // if it is not last thread then allocate file pointer to end of File
        
	long int linesSort = posEnd - posStart;              // No of lines sorted by this function
        long int memAlot = ( MemFree  * (1024/blockSize) )/( NumPTH * 2 ); // Allocate memory slot for sorting 
	//printf("\n %ld -- %ld \n",linesSort,memAlot);	
	if(linesSort < ( memAlot))            // if memory slot is smaller then file size 
	{
		fseek(filePointer, posStart * blockSize, SEEK_SET); // set filepointer based on every thread 
                
                char **sortBuffer = (char **) malloc(linesSort*sizeof(char*)); // allocate memory in sortBuffer to store file 
		
                if ( sortBuffer == NULL  ) {
                	fprintf(stderr, "mergeSortFile: Couldn't allocate memory for sortBuffer1. \n");
                	exit(EXIT_FAILURE);
                }
                
		for( i = 0; i < linesSort; i++ )   // allocate memory to store 2d sortbuffer data
		{
			sortBuffer[i] = (char * )malloc( blockSize * sizeof( char ));
                        
			if ( sortBuffer[ i ] == NULL  ) {
                		fprintf(stderr, "mergeSortFile: Couldn't allocate memory for sortBuffer[%d].\n",i);
                		exit(EXIT_FAILURE);
                	}
			fread( sortBuffer[ i ],blockSize, 1 , filePointer );
		}
		 
		mergeSort( sortBuffer, 0, linesSort - 1 );
		FILE *fP;
		
                char file[ 20 ];
		
                sprintf( file, "Thread%d.txt", nth );
                
		if (!( fP = fopen( file, "w" ))) {
                	fprintf(stderr, "mergeSortFile: File opened failed \n");
                        exit(EXIT_FAILURE);
		}
		for( i = 0; i < linesSort; i++ )
		{
                	fwrite( sortBuffer[ i ], 1, blockSize, fP );
		}
		fclose( fP );
		for( i = 0; i < linesSort; i++ )
			free( sortBuffer[ i ] );
                free(sortBuffer);
		if(++Csort == 2 ){
			printf("\n\n In Memory Sorting Complete  \n ");
			SortEnd = clock();
			mergeStart = clock();
		}

	}
	else
	{
		long int totalFileSize = linesSort;

		linesSort = memAlot ;
		long int Tcount = ( totalFileSize / ( linesSort )) + 1;
		char *file[ Tcount ];
		for ( i = 0; i < Tcount; i++ ){
                        
                	file[ i ] =(char *) malloc( blockSize * sizeof( char ));
                	if ( file[ i ] == NULL ) {
                	        fprintf( stderr, "Couldn't allocate memory for files\n");
                        	exit(EXIT_FAILURE);
                 	}
        	}
		for(int si = 0;si<Tcount;si++){
		totalFileSize = totalFileSize - linesSort;
		if(totalFileSize <0)
		{
			linesSort = linesSort + totalFileSize;
		}
		fseek(filePointer, posStart*blockSize, SEEK_SET); // set filepointer for each thread
		char **sortBuffer = (char **) malloc(linesSort*sizeof(char*)); // allocate memory in sortBuffer to store file 
		
                if ( sortBuffer == NULL  ) {
                	fprintf(stderr, "mergeSortFile: Couldn't allocate memory for sortBuffer. \n");
                	exit(EXIT_FAILURE);
                }
                for(i =0;i<linesSort;i++)
		{
			sortBuffer[i] = (char *)malloc(blockSize*sizeof(char));
			if ( sortBuffer[i] == NULL  ) {
                		fprintf(stderr, "mergeSortFile: Couldn't allocate memory for sortBuffer[%d].\n",i);
                		exit(EXIT_FAILURE);
                	}
			fread(sortBuffer[i],blockSize,1,filePointer);
		}
		
                mergeSort( sortBuffer, 0, linesSort - 1 );
		FILE *fP;
		sprintf( file[ si ], "Thread%d-%d.txt", nth, si );
                
		if ( !( fP = fopen( file[si], "w" ))) {
                	printf("\n mergeSortFile: File opened failed\n");
                	exit(EXIT_FAILURE);
		}
		for( i = 0;i < linesSort; i++ )
		{
                	fwrite( sortBuffer[ i ], 1 , blockSize, fP );
		}
		fclose( fP );
		for(i =0; i < linesSort; i++ )
			free( sortBuffer[ i ] );
                	free(sortBuffer);
		}
                
		if(++Csort == 2 ){
			printf("\n\n In Memory Sorting Complete  \n ");
			SortEnd = clock();
			mergeStart = clock();
			printf( " \n Thread Level Merging Started  \n " );
		}
		
                char fileall[ 100 ];
                
                sprintf( fileall, "Thread%d.txt", nth );
		mergeAllSortFile( file, Tcount, fileall, nth );
	
		for (int j = 0; j < Tcount; j++){
			
                        free(file[j]);
		}	
	}
	
    	fclose( filePointer );
}

