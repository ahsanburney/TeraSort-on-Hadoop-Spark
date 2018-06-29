
/* C program for Merge Sort */
#include"TeraMPISort.h"

int main( int argc, char **argv)
{
	int P,rank,i;
  	MPI_Init( &argc, &argv  );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); // it provides rank of mpi node
	MPI_Comm_size( MPI_COMM_WORLD, &P );    // Total no of nodes 
	MPI_Status *status;
	MPI_Request *request;

	status = (MPI_Status *) malloc(P * sizeof(MPI_Status));
	request = (MPI_Request *) malloc(P * sizeof(MPI_Request));
	
	double cpu_time_used;
	char outputFilename[100] = "output.txt";
	NumProc = 2;
	if(argc<3)
	{	
		printf(" Please Enter given Format : ./run input.txt output.txt ");
		return 0;
	}
	strcpy(inputFilename,argv[1]);  // first argument inputfile name
	strcpy(outputFilename,argv[2]);  // Second argument outfile name
	NumProc = P;	// third argument number of threads
	MemFree = GetRamInKB(0); // find total free memory
	double splitTime;
	if( rank == MASTER )
	{	
	
		printf("\n***\t MPI Terasort using MPI and MergeSort\t ***\n");
	
	
		printf("\nInput file name: %s ", inputFilename);
	
		printf("\nOutput file name: %s ",outputFilename);

		printf("\nNumber of Nodes: %d \n",NumProc);

	 	FILE *filePointer;

        	int blockSize = 100;  // line size in gensort program generate file

        	if(!(filePointer = fopen(inputFilename, "r")))  // open input file
        	{
               		 printf("\n mergeSortFile: Input1 File open failed.\n");
                	exit(EXIT_FAILURE);
        	}

        	fseek(filePointer,0L,SEEK_END);  // calculate total size of file by moving file pointer to end
        
		long int  TotalFileSize=ftell(filePointer); 
	
		fclose(filePointer);

		printf("\nInput File Size       : %.2f GB ",((double)TotalFileSize)/(1024*1024*1024));
	
		printf("\nTotal Memory(RAM)     : %.2f GB ",((double)GetRamInKB(2))/(1024*1024));
		printf("\nAvailable Memory(RAM) : %.2f GB ",((double)MemFree)/(1024*1024));
	 	long int memslot = ( MemFree   )/( NumProc * 2 );
        	long int chunks = NumProc * ceil(((((double)TotalFileSize)/1024)/(memslot*NumProc)));
	
		start = clock();
		SplitFile( TotalFileSize/100,(MemFree*((double)(1024/100))));
		splitTime = ((double) (clock() - start)) / CLOCKS_PER_SEC;
		printf("Completed splitting files ! \n " );  
        	printf("\nTotal No. chunks Created Based on Avaliable Memory & Nodes  : %d \n\n",(int)chunks);

	}  
        
	mergeMPISortFile(rank);  // All Processor call function mergeSortFile and sends Processor count
       	
	if( rank == MASTER )
	{
	 	end = clock();
		char *files[NumProc];
        
		for (i = 0; i <NumProc; i++){  //this loop create list of files to sort 
			files[i] =(char *) malloc(100*sizeof(char));
                	if ( files[i] == NULL ) {
                        	fprintf(stderr, "main: Couldn't allocate memory for files.\n");
                        	exit(EXIT_FAILURE);
        		}
			char rmfiles[50];
			sprintf(files[i],"Thread%d.txt",i);
		}
	
		printf("\n Main Thread : Merging Started for Thread Chunks\n\n");
		mergeMPIAllSortFile(files,NumProc,outputFilename,0); // call function to merge all chunks 
		
		for( i = 0;i < NumProc; i++ )
		{
			free( files[i]);
		}
 		end = clock();
        	printf("----------------------------------------------------------------------------");
     		cpu_time_used = ((double) (SortEnd - start)) / CLOCKS_PER_SEC;
     		printf("\n\t Splitting Time for chunks          = %lf sec \n  ", splitTime);
     	
     		cpu_time_used = ((double) (SortEnd - start)) / CLOCKS_PER_SEC;
     		printf("\n\t Sorting Time for chunks            = %lf sec \n  ", cpu_time_used);
		
		cpu_time_used = ((double) (end - mergeStart)) / CLOCKS_PER_SEC;
		printf("\n\t Merging Time for Sorted chunks     = %lf sec  \n", cpu_time_used);
        
        	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     		printf("\n\n\t Total MPI Tera Sorting Time    = %lf sec  \n", cpu_time_used);
        	printf("----------------------------------------------------------------------------\n");
        }
	MPI_Finalize();
 return 0;

}

void SplitFile(long int TotalFileSize,long int memfree)
{
	char infile[100];
	FILE *filePointer,*fP;
	int i;
	if(!(filePointer = fopen(inputFilename, "r")))  // open input file
        {
                printf("\n mergeSortFile: Input1 File open failed.\n");
                exit(EXIT_FAILURE);
        }
	//printf("\n ---- FileSize %ld -- %ld \n",TotalFileSize,memfree);	
	
	printf("\n\nInitializing Sorting of Chunks\n ");
	{
		char *buffer = (char *)malloc(100*sizeof(char));
                for(int k=0;k<NumProc;k++)
                {
                        sprintf(infile,"input%d",k);
                        if (!( fP = fopen( infile, "w" ))) {
                                fprintf(stderr, "mergeSortFile: File opened failed \n");
                                exit(EXIT_FAILURE);
                        }
                        for( i = 0; i <(TotalFileSize/(NumProc)) ; i++ )
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

void mergeMPISortFile(int param )
{
        int nth = param;

	printf("******* Processor - %d Started Sorting ***********\n",nth+1);

	//printf(" Thread Count = %d ",nth);
	int i;
	FILE *filePointer;
	
        int blockSize = 100;  // line size in gensort program generate file
	if(!(filePointer = fopen(inputFilename, "r")))  // open input file
     	{
                printf("\n mergeSortFile: Input File open failed.\n");
                exit(EXIT_FAILURE);
    	}
        
	fseek(filePointer,0L,SEEK_END);  // calculate total size of file by moving file pointer to end
      	
        long int TotalLines=ftell(filePointer)/blockSize;   // calculate total lines in file
        
	long int posStart = (TotalLines/NumProc)*nth;  ;//Set File pointer Start based on thread count
	
        long int posEnd;  
	
        if(nth == NumProc-1)                             // if this function called by last thread then rest of lines is alloted to function
		posEnd   = TotalLines;                  // total no of lines 
	else
		posEnd   = (TotalLines/NumProc)*(nth+1); // if it is not last thread then allocate file pointer to end of File
        
	long int linesSort = posEnd - posStart;              // No of lines sorted by this function
        long int memAlot = ( MemFree )*((double) (1024 /( NumProc * 4 ))); // Allocate memory slot for sorting 
		
	if(linesSort < ( memAlot/blockSize))            // if memory slot is smaller then file size 
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
		for( i = 0; i < linesSort; i++ )
			free( sortBuffer[ i ] );
                free(sortBuffer);
		if( nth == MASTER )
		{
			printf("\n\n In Memory Sorting Complete  \n ");
			SortEnd = clock();
			mergeStart = clock();
		}
		fclose( fP );

	}
	else
	{
		printf("Lines To sort \n");
		long int totalFileSize = linesSort;

		linesSort = memAlot / blockSize;
		int Tcount = ( totalFileSize / ( linesSort )) + 1;
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
		if(nth==MASTER)        
		{
			printf("\n\n In Memory Sorting Complete  \n ");
			SortEnd = clock();
			mergeStart = clock();
			printf( " \n Thread Level Merging Started  \n " );
		}
		
                char fileall[ 100 ];
                
                sprintf( fileall, "Thread%d.txt", nth );
		mergeMPIAllSortFile( file, Tcount, fileall, nth );
	
		for (int j = 0; j < Tcount; j++){
			
                        free(file[j]);
		}	
	}
    	fclose( filePointer );

}

