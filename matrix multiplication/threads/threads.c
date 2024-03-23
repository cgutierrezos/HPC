#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

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

int main(int argc, char *argv[]) {
    int N = 0, num_threads = 1;

    // Parse command line arguments
    if (argc != 3) {
        printf("Usage: %s <N> <num_threads>\n", argv[0]);
        exit(1);
    }

    N = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    if (N <= 0 || num_threads <= 0) {
        printf("Invalid arguments\n");
        exit(1);
    }

    // Memory allocation
    int **A, **B, **C;
    allocateMatrices(&A, &B, &C, N);

    // Filling matrices
    fillMatrices(A, B, C, N);

    clock_t start, end;
    double cpu_time_used;

    start = clock(); // Start the stopwatch

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

    end = clock(); // Stop the stopwatch
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate total CPU time

    printf("%d, %d, %f\n", num_threads, N, cpu_time_used);

    // Deallocate memory
    freeMatrices(A, B, C, N);

    return 0;
}


