#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Incluye la declaración de strcmp
#include <omp.h> // Incluye la declaración de OpenMP

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
        (*C)[i] = (int *)malloc(N * sizeof(int));
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

void MultiplyMatrices(int **A, int **B, int **C, int N, double *totalMaxExecutionTime, int numberOfThreads) {
    double maxExecutionTime = 0.0;

    // Set the number of threads
    omp_set_num_threads(numberOfThreads);

    #pragma omp parallel
    {
        double start, end;
        start = omp_get_wtime();

        #pragma omp for
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int sum = 0;
                for (int k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }

        end = omp_get_wtime();
        double executionTime = end - start;

        #pragma omp critical
        {
            if (executionTime > maxExecutionTime) {
                maxExecutionTime = executionTime;
            }
        }
    }

    *totalMaxExecutionTime = maxExecutionTime;
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

void getCommands(int argc, char *argv[], int *N, int *verbose, int *numberOfThreads) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1;
        } else if (i == 1) {
            *N = atoi(argv[i]);
        } else if (i == 2) {
            *numberOfThreads = atoi(argv[i]);
        }
    }

    if (*N == 0) {
        printf("Usage: %s <N> <numberOfThreads> [-v|--verbose]\n", argv[0]);
        exit(1);
    }

    if (*numberOfThreads == 0) {
        printf("Usage: %s <N> <numberOfThreads> [-v|--verbose]\n", argv[0]);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int N = 0, verbose = 0, numberOfThreads = 0;
    getCommands(argc, argv, &N, &verbose, &numberOfThreads);

    int **A, **B, **C;
    allocateMatrices(&A, &B, &C, N);
    fillMatrices(A, B, C, N);

    clock_t startTotal, endTotal;
    double totalCpuTimeUsed, totalMaxExecutionTime = 0.0, totalMainThread = 0.0;

    startTotal = clock();
    MultiplyMatrices(A, B, C, N, &totalMaxExecutionTime, numberOfThreads);
    endTotal = clock();

    totalMainThread = ((double) (endTotal - startTotal)) / CLOCKS_PER_SEC;
    totalCpuTimeUsed = totalMaxExecutionTime + totalMainThread;

    if (verbose) {
        printMatrices(A, B, C, N);
        printf("Total CPU Time: %f\n", totalCpuTimeUsed);
        printf("Total Max Execution Time: %f\n", totalMaxExecutionTime);
    } else {
        printf("%d, %d, %f\n", numberOfThreads, N, totalCpuTimeUsed);
    }

    freeMatrices(A, B, C, N);

    return 0;
}
