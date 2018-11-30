#include <stdio.h>
#include <stdlib.h>


#define TRUE 1
#define FALSE 0

// function prototypes
int check_if_sorted(int A[], int n);
void generate_random_array(int A[], int n, int seed);
void serial_mergesort(int A[], int p, int r); 
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
//new mergesort
void parallel_mergesort(int A[], int p, int r, int numthreads);
double getMilliSeconds();

/*
 * generate_random_array(int A[], int n, int seed):
 *
 * description: Generate random integers in the range [0,RANGE]
 *              and store in A[1..n]
 */

#define RANGE 1000000

void generate_random_array(int A[], int n, int seed)
{
    int i;

    srandom(seed);
    for (i=1; i<=n; i++)
        A[i] = random()%RANGE;
}


/*
 * check_if_sorted(int A[], int n):
 *
 * description: returns TRUE if A[1..n] are sorted in nondecreasing order
 *              otherwise returns FALSE
 */     

int check_if_sorted(int A[], int n) 
{
    int i=0;

    for (i=1; i<n; i++) {
        if (A[i] > A[i+1]) {
            return FALSE;
        }
    }
    return TRUE;
}


int main(int argc, char **argv) {
    int n;
    int seed;

    if (argc < 3) { // there must be at least one command-line argument
            fprintf(stderr, "Usage: %s <input size> <number of threads> [<seed>] \n", argv[0]);
            exit(1);
    }
    
    n = atoi(argv[1]); //default seed if no arg for it
    seed = 1;
    if (argc == 4)
        seed = atoi(argv[3]);
        
    int *A = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]
    int numthreads = atoi(argv[2]);
        
    // generate random input
    
    generate_random_array(A,n, seed);
    
    double start_time;
    double sorting_time;

    start_time = getMilliSeconds();
    //call parallel mergesort instead of serial
    parallel_mergesort(A, 1, n, numthreads);
    sorting_time = getMilliSeconds() - start_time;
    
    // print results if correctly sorted otherwise cry foul and exit
    if (check_if_sorted(A,n)) {
        printf("Sorting %d elements took %4.2lf seconds.\n", n,  sorting_time/1000.0);
    } else { 
        printf("%s: sorting failed!!!!\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	//free array
    free(A);
    exit(EXIT_SUCCESS); 
} 
