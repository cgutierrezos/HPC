#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printMatrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

void printMatrices(int *A, int *B, int *C, int N) {
    printf("A MATRIX:\n");
    printMatrix(A, N, N);

    printf("\n\n");

    printf("B MATRIX:\n");
    printMatrix(B, N, N);

    printf("\n\n");

    printf("C MATRIX:\n");
    printMatrix(C, N, N);

    printf("\n\n");
}

void allocateMatrices(int **A, int **B, int **C, int N) {
    *A = (int *)malloc(N * N * sizeof(int));
    *B = (int *)malloc(N * N * sizeof(int));
    *C = (int *)malloc(N * N * sizeof(int));
    
    if (*A == NULL || *B == NULL || *C == NULL) {
        fprintf(stderr, "Error allocating memory for matrices\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

void fillMatrices(int *A, int *B, int *C, int N) {
    srand(time(NULL) + rand()); // Mejor inicialización de la semilla
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = rand() % 100;
            B[i * N + j] = rand() % 100;
            C[i * N + j] = 0;
        }
    }
}

void MultiplyMatrices(int *A, int *B, int *C, int N, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

void freeMatrices(int *A, int *B, int *C) {
    if (A) free(A);
    if (B) free(B);
    if (C) free(C);
}

void getCommands(int argc, char *argv[], int *N, int *verbose) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1;
        } else if (i == 1) {
            *N = atoi(argv[i]);
        }
    }

    if (*N == 0) {
        printf("Usage: %s <N> [-v|--verbose]\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

int main(int argc, char *argv[]) {
    int N = 0, verbose = 0;
    getCommands(argc, argv, &N, &verbose);

    int rank, size;
    int *A = NULL, *B = NULL, *C = NULL, *sub_A = NULL, *sub_C = NULL;
    int rows, remainder, *sendcounts, *displs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows = N / size;
    remainder = N % size;

    sendcounts = (int *)malloc(size * sizeof(int));
    displs = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        sendcounts[i] = (i < remainder) ? (rows + 1) * N : rows * N;
        displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
    }

    sub_A = (int *)malloc(sendcounts[rank] * sizeof(int));
    sub_C = (int *)malloc(sendcounts[rank] * sizeof(int));
    B = (int *)malloc(N * N * sizeof(int));

    if (sub_A == NULL || sub_C == NULL || B == NULL) {
        fprintf(stderr, "Error allocating memory for sub-matrices\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        allocateMatrices(&A, &B, &C, N);
        fillMatrices(A, B, C, N);
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(A, sendcounts, displs, MPI_INT, sub_A, sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);  // Ensures all processes start together
    double startTotal = MPI_Wtime();

    MultiplyMatrices(sub_A, B, sub_C, N, sendcounts[rank] / N);

    MPI_Barrier(MPI_COMM_WORLD);  // Ensures all processes finish together
    double endTotal = MPI_Wtime();

    MPI_Gatherv(sub_C, sendcounts[rank], MPI_INT, C, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (verbose) {
            printMatrices(A, B, C, N);
        }
        printf("Number of Processes: %d, Matrix Size: %d, Time Taken: %f seconds\n", size, N, endTotal - startTotal);
        freeMatrices(A, B, C);
    }

    // free(sub_A);
    // free(sub_C);
    // free(B);
    // free(sendcounts);
    // free(displs);

    MPI_Finalize();
    return 0;
}
