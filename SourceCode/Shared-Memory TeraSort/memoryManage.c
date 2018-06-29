#include <stdio.h>
#include <errno.h>
#include<stdlib.h>

#include <sys/types.h>
#include <sys/sysctl.h>

// this function get available memory

int GetRamInKB( int p )
{
	FILE *meminfo = fopen("/proc/meminfo", "r");
    	if( meminfo == NULL)
		printf("Error");
	char line[ 256 ];
	if(p !=2)
	{ 
    		while( fgets( line, sizeof( line ), meminfo ))  // loop read file /proc/meminfo
    		{	
        		int ram;
        		if( sscanf( line, "MemAvailable: %d kB", &ram ) == 1 )
        		{
        			fclose(meminfo);  // when it finds available memory it breaks the loop and return value 
            			return ram;
        		}
    		}
    	}
	else
	{
		while( fgets( line, sizeof( line ), meminfo ))  // loop read file /proc/meminfo
                {
                        int ram;
                        if( sscanf( line, "MemTotal: %d kB", &ram ) == 1 )
                        {
                                fclose(meminfo);  // when it finds available memory it breaks the loop and return value
                                return ram;
                        }
                }
	}
	fclose(meminfo);
    	return -1;
}
