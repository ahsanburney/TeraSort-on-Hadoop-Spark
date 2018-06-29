#include<string.h>
// This Function Merge Two arrays based on string values

void merge(char *buffer[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    char **L, **R;
        L = (char **)malloc(n1 * sizeof(char *));
        R =(char **)malloc(n2 * sizeof(char *));
        
		if (L == NULL || R == NULL  ) {
			fprintf(stderr, "Couldn't allocate memory for thread.\n");
    			exit(EXIT_FAILURE);
        }

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = buffer[l + i];
    for (j = 0; j < n2; j++)
        R[j] = buffer[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        //if ( sequential_Read(L[i],R[j])<0)
        if ( strncmp(L[i],R[j],10)<0)
        {
            buffer[k] = L[i];
            i++;
        }
        else
        {
            buffer[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        buffer[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        buffer[k] = R[j];
        j++;
        k++;
    }
        free(L);
        free(R);
}

//This function divide array into into parts, pass to merge function

void mergeSort( char *buffer[],int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
        // Sort first and second halves
        mergeSort(buffer, l, m);
        mergeSort(buffer, m+1, r);
        merge(buffer, l, m, r);
    }
}
