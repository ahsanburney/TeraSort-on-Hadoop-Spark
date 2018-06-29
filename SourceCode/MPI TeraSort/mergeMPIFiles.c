#include<string.h>

//This function merge two files into output files based on ascending order

void mergeFile(FILE *fP1,FILE *fP2,FILE *fP3)
{
        int blockSize = 100; //gensort generated lines of data in which every line 100 bytes
		
		char *buffer1 = (char *)malloc(blockSize * sizeof(char));
        if ( buffer1 == NULL  ) {
                fprintf(stderr, "mergeFile: Couldn't allocate memory for buffer1.\n");
                exit(EXIT_FAILURE);
        }
		
		char *buffer2 = (char *)malloc(blockSize * sizeof(char));
        if ( buffer2 == NULL  ) {
                fprintf(stderr, "mergeFile: Couldn't allocate memory for buffer2.\n");
                exit(EXIT_FAILURE);
        }
		
		fseek(fP1,0L,SEEK_END);   		// set pointer to end of the file
        int TL1=ftell(fP1)/blockSize;   // this gives the number of lines files contains
        fseek(fP1,0L,SEEK_SET);  		// now set pointer to start of the file
	
		fseek(fP2,0L,SEEK_END);  		// set pointer to end of the file
        int TL2=ftell(fP2)/blockSize;  	// this gives the number of lines files contains
        fseek(fP2,0L,SEEK_SET); 		// now set pointer to start of the file
		int fc = (TL1>(GetRamInKB(1)/3))?0:1;
		
		if( TL1 == 0 || TL2 == 0 )  	// if any file is empty return,  merge won't happen
			return;
		
		fread( buffer1, blockSize, 1, fP1 ); TL1--;  // Read first line from first file
		
		fread( buffer2, blockSize, 1, fP2 ); TL2--;  // Read first line from second file
		
		int k =0;	
	while( TL1 != 0 && TL2 != 0 & fc ) 		    // check until while if any file become empty
	{
		if( strncmp( buffer1, buffer2,5 ) < 0 ) // compare only first 10 bytes in both lines because in gensort first 10bytes value is different
		{
			fwrite(buffer1,1,blockSize,fP3);    // if buffer1 is smaller than buffer2, then write buffer1 into output file
			fread(buffer1,blockSize,1,fP1);TL1--;  // read buffer1 from file1 and decrement counter
		}
		else
		{
			fwrite(buffer2,1,blockSize,fP3);   // if buffer2 is smaller than buffer1, then write buffer2 into output file
			fread(buffer2,blockSize,1,fP2);TL2--; // read buffer2 from file2 and decrement counter
		
		}
	}
		
	if(strncmp(buffer1,buffer2,10)<0)     // when while loop terminate both buffer1 and buffer2 have data 
        {
         	fwrite(buffer1,1,blockSize,fP3); // if buffer1 is smaller than buffer2, then write buffer1 into output file
         	fwrite(buffer2,1,blockSize,fP3); // then write buffer2 into output file
        }
        else
        {
                fwrite(buffer2,1,blockSize,fP3); // if buffer2 is smaller than buffer1, then write buffer1 into output file
         	fwrite(buffer1,1,blockSize,fP3); // then write buffer1 into output file
        }
	while(TL1!=0)  // check file1 still have some data then merge into ouput file 
	{
		fread(buffer1,blockSize,1,fP1);TL1--;  // read data from file1 
		fwrite(buffer1,1,blockSize,fP3);   // write data into output file
	}
	while(TL2!=0)  // check file1 still have some data then merge into ouput file  
	{
         fread(buffer2,blockSize,1,fP2);TL2--;  //read data from file1 
		 fwrite(buffer2,1,blockSize,fP3);   // write dtat into output file 
	}
	free(buffer1);  //clear buffer1
	free(buffer2);  //clear buffer2
}


void mergeMPIAllSortFile(char *files[], int size, char outputFile[],int C)
{
	
	register int blockSize = 100;
	register int i=0,j,filePosition;
	FILE *fP1;   // input file pointer1
	FILE *fP2;   // input filepointer2 
	FILE *fP3;   // output filepointer3
	char *tmp1;  //Temperory variable to store file1 
	char *tmp2;  //Temperory variable to store file2
	tmp1 = ( char * )malloc( blockSize * sizeof( char ));
	if ( tmp1 == NULL  ) {
		fprintf(stderr, "mergeAllSortFile: Couldn't allocate memory for tmp1.\n");
		exit(EXIT_FAILURE);
        }
		
	tmp2 = (char *)malloc(blockSize * sizeof(char));  
        if ( tmp2 == NULL  ) {
                fprintf(stderr, "mergeAllSortFile: Couldn't allocate memory for tmp2.\n");
                exit(EXIT_FAILURE);
        }
	strcpy(tmp1,files[0]);  // store filename1 
	
	sprintf( tmp2, "tmp%d.txt", C );  // store data into temperory output folder
	
	char str[100];
	
        for( i = 1; i < size; i++ )
		{
			if ( ! ( fP1 = fopen( tmp1, "r" ) ) ) {   // open firts input file
                	printf("\n mergeAllSortFile: File open failed.\n");
                	exit(EXIT_FAILURE);
            	}
		
		if (!(fP2 = fopen(files[i],"r"))) {   // open second input file
                	printf("\n mergeAllSortFile: File open failed.\n");
                	exit(EXIT_FAILURE);
            	}
	 	if (!(fP3 = fopen(tmp2,"w"))) {    // open output file
          	      printf("\n mergeAllSortFile: File open failed.\n");
                	exit(EXIT_FAILURE);
            	}
		mergeFile(fP1,fP2,fP3);   // merge file1 and file2 into output file.
		fclose(fP1);	  // close filepointer1
		fclose(fP2);		// close filepointer2
		fclose(fP3);		// close filepointer2 
		sprintf(str,"rm %s ",files[i]);  // remove merged files
		system(str);  
		sprintf(str,"mv %s %s ",tmp2,files[0]); // mv temperory file as input file for next while loop
		system(str);
		}
		sprintf(str,"mv %s %s ",files[0],outputFile);  // finaly move tempory into output file
		system(str);

}

