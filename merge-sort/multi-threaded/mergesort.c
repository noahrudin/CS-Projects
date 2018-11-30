#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

// function prototypes
void serial_mergesort(int A[], int p, int r); 
void merge(int A[], int p, int x, int r);
void insertion_sort(int A[], int p, int r);
void parallel_mergesort(int[], int, int, int);
struct args* sub_array;

const int INSERTION_SORT_THRESHOLD = 100; //based on trial and error


/*
 * insertion_sort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void insertion_sort(int A[], int p, int r) 
{
    int j;

    for (j=p+1; j<=r; j++) {
        int key = A[j];
        int i = j-1;
        while ((i > p-1) && (A[i] > key)) { 
            A[i+1] = A[i];
            i--;
        }
        A[i+1] = key;
    }
}

        

/*
 * serial_mergesort(int A[], int p, int r):
 *
 * description: Sort the section of the array A[p..r].
 */
void serial_mergesort(int A[], int p, int r) 
{
    if (r-p+1 <= INSERTION_SORT_THRESHOLD)  {
            insertion_sort(A,p,r);
    } else {
        int x = (p+r)/2;
        serial_mergesort(A, p, x);
        serial_mergesort(A, x+1, r);
        merge(A, p, x, r);
    }
}



void merge(int A[], int p, int x, int r) 
{
    int *B = (int *) malloc(sizeof(int) * (r-p+1));
        
    int i = p;
    int j = x+1;
    int k = 0;
    int l;
        
    // as long as both lists have unexamined elements
    // this loop keeps executing.
    while ((i <= x) && (j <= r)) {
        if (A[i] < A[j]) {
            B[k] = A[i];
            i++;
        } else {
            B[k] = A[j];
            j++;
        }
        k++;
    }
        
    // now only at most one list has unprocessed elements.
        
    if (i <= x) { 
        // copy remaining elements from the first list
        for (l=i; l<=x; l++) {
            B[k] = A[l];
            k++;
        }
    } else {
        // copy remaining elements from the second list
        for (l=j; l<=r; l++) {
            B[k] = A[l];
            k++;
        }
    }
        
    // copy merged output from array B back to array A
    k=0;
    for (l=p; l<=r; l++) {
        A[l] = B[k];
        k++;
    }

    free(B);
}

//create a struct that contains the sub-array, beginning, and end
struct args {
    int* A;
    int p;
    int r;
};


void* sort_args(void* arg)
{
    int* A = ((struct args*)arg) -> A;
    int p = ((struct args*)arg) -> p;
    int r = ((struct args*)arg) -> r;

    serial_mergesort(A, p, r);
    
    return NULL;
}

/*
 * parallel_mergesort(int A[], int p, int r, int numthreads)
 * Sorts array A with concurrency via multiple threads. Typically the optimum thread count 
 * is the number of cores in the machine.
 * @param A[](sub-array), p(start element of sub-array), r(end element of sub-array), numthreads(number of threads)
 */
void parallel_mergesort(int A[], int p, int r, int numthreads) 
{
	//divide array by number of threads
    int arraylength = r/numthreads;
    
	int head;
    int tail;
	int i = 0;
    int x;

	//malloc the amount of memory needed for arg_struct
	struct args** arg_struct = (struct args**)malloc(sizeof(struct args*) * numthreads);
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t)*numthreads);

	while(i< numthreads)
    {
        head = i * arraylength; 
        tail = head + arraylength - 1;
		
        if(tail > r  || (tail < r && i == numthreads - 1))
            tail = r;
        
		//make allocation for smaller sub-array
        sub_array = (struct args*)malloc(sizeof(struct args));

		//creating array with given start and end
        sub_array -> A = A;
        sub_array -> p = head;
        sub_array -> r = tail;

		//add the array to list of arrays
        arg_struct[i] = sub_array;
        
		//start process
        pthread_create(&tid[i], NULL, sort_args, (void*)arg_struct[i]);
		i++;
    }
    i = 0;

	while(i< numthreads)
    {
		//finishes as target thread completes execution
        pthread_join(tid[i], NULL);
		i++;
    }
    i = 0;

	while (i<numthreads)
    {   
        if(i < numthreads - 1)
        {   
			//piece sub-arrays back together
            tail = arg_struct[i + 1] -> r;
            x = arg_struct[i] -> r;
        
            merge(A, 1, x, tail);
        }
		//free memory for specific array in the struct
        free(arg_struct[i]);
		i++;
    }
	i = 0;

	//free the arg struct memory
    free(arg_struct);
    free(tid);
}