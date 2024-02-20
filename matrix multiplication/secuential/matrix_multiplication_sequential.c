#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int **matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("N value not provided");
        return -1;
    }

    int N = atoi(argv[1]); // Converts the command line argument to an integer

    srand(time(NULL));

    // Memory allocation
    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    int **C = (int **)malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)calloc(N, sizeof(int)); // Initializes to 0
    }

    // Filling matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100; 
            B[i][j] = rand() % 100;
            C[i][j] = 0;
        }
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock(); // Starts the stopwatch

    // Matrix multiplication
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = clock(); // Stops the stopwatch

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Printing results
    printf("%d, %f\n", N, cpu_time_used);
    

    // Freeing memory
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}