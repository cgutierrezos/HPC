#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printMatrix(int **matrix, int N) {
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void printMatrices(int **A, int **B, int **C, int N){

    printf("A MATRIX:\n");
    printMatrix(A, N);

    printf("\n\n");

    printf("B MATRIX:\n");
    printMatrix(B, N);

    printf("\n\n");

    printf("C MATRIX:\n");
    printMatrix(C, N);

    printf("\n\n");
}


void allocateMatrices(int ***A, int ***B, int ***C, int N) {
    
    *A = (int **)malloc(N * sizeof(int *));
    *B = (int **)malloc(N * sizeof(int *));
    *C = (int **)malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++) {
        (*A)[i] = (int *)malloc(N * sizeof(int));
        (*B)[i] = (int *)malloc(N * sizeof(int));
        (*C)[i] = (int *)malloc(N * sizeof(int)); // You can use calloc instead if you want to initialize to 0
    }
}

void fillMatrices(int **A, int **B, int **C, int N){
    
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            
            A[i][j] = rand() % 100; 
            B[i][j] = rand() % 100;
            C[i][j] = 0;
            
        }
    }

}

void MultiplyMatrices(int **A, int **B, int **C, int N){
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void freeMatrices(int **A, int **B, int **C, int N){
    
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);

}


void getCommands(int argc, char *argv[], int *N, int *verbose){

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1; // Enable verbose output
        } else if (i == 1) {
            *N = atoi(argv[i]); // First positional argument is matrix size
        } 
    }

    if (*N == 0) {
        printf("Usage: %s <N> [-v|--verbose]\n", argv[0]);
        exit(1);
    }


}


int main(int argc, char *argv[]) {

    int N = 0, verbose = 0;

    //Process command line arguments
    getCommands(argc, argv, &N, &verbose);
    
    // Memory allocation
    int **A, **B, **C;
    allocateMatrices(&A, &B, &C, N);
    
    // Filling matrices
    fillMatrices(A, B, C, N);

    
    clock_t start, end;
    double cpu_time_used;

    start = clock(); // Starts the stopwatch

    MultiplyMatrices(A, B, C, N);

    end = clock(); // Stops the stopwatch

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;


    if(verbose){
        printMatrices(A, B, C, N);
        // Printing results

        printf("Time Clock to process: %f", cpu_time_used);
    }else{

        printf("%d, %f\n", N, cpu_time_used);
    }


    // Freeing memory
    freeMatrices(A, B, C, N);
    
    return 0;
}