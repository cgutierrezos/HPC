#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

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

void fillMatrices(int **A, int **B, int **C, int N) {
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0;
        }
    }
}

void freeMatrices(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);
}

typedef struct {
    int **A;
    int **B;
    int **C;
    int start_row;
    int end_row;
    int N;
} ThreadData;

void *multiplyMatrices(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < data->N; j++) {
            for (int k = 0; k < data->N; k++) {
                data->C[i][j] += data->A[i][k] * data->B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

void getCommands(int argc, char *argv[], int *N, int *num_threads, int *verbose){

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1; // Enable verbose output
        } else if (i == 1) {
            *N = atoi(argv[i]); // First positional argument is matrix size
        } else if (i == 2) {
            *num_threads = atoi(argv[i]); // First positional argument is matrix size
        } 
    }

    if (*N == 0) {
        printf("Usage: %s <N> <num_threads> [-v|--verbose]\n", argv[0]);
        exit(1);
    }

    if (*num_threads == 0) {
        printf("Usage: %s <N> <num_threads> [-v|--verbose]\n", argv[0]);
        exit(1);
    }


}

int main(int argc, char *argv[]) {

    int N = 0, verbose = 0, num_threads = 0;
    
    //Process command line arguments
    getCommands(argc, argv, &N, &num_threads, &verbose);
    
    // Memory allocation
    int **A, **B, **C;
    allocateMatrices(&A, &B, &C, N);

    // Filling matrices
    fillMatrices(A, B, C, N);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    //clock_t start, end;
    double cpu_time_used;

    //start = clock(); // Start the stopwatch

    // Creating thread data and threads
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int rows_per_thread = N / num_threads;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;
        thread_data[i].N = N;
        if (i == num_threads - 1) {
            thread_data[i].end_row = N; // Last thread handles remaining rows
        }
        pthread_create(&threads[i], NULL, multiplyMatrices, (void *)&thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    //end = clock(); // Stop the stopwatch
    //cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate total CPU time

    gettimeofday(&end, NULL);

    double time_spent = ((end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0);

    if(verbose){
        printMatrices(A, B, C, N);
        // Printing results

        printf("Time Clock to process: %f", time_spent);
    }else{

        printf("%d, %d, %f\n", num_threads, N, time_spent);
    }

    // Deallocate memory
    freeMatrices(A, B, C, N);

    return 0;
}


