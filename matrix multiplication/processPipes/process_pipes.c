#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

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

void multiplyMatrices(int **A, int **B, int **C, int start_row, int end_row, int N) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int N = 0, num_processes = 1;

    // Parse command line arguments
    if (argc != 3) {
        printf("Usage: %s <N> <num_processes>\n", argv[0]);
        exit(1);
    }

    N = atoi(argv[1]);
    num_processes = atoi(argv[2]);

    if (N <= 0 || num_processes <= 0) {
        printf("Invalid arguments\n");
        exit(1);
    }

    // Memory allocation
    int **A, **B, **C;
    allocateMatrices(&A, &B, &C, N);

    // Filling matrices
    fillMatrices(A, B, C, N);

    clock_t start, end;
    double cpu_time_used = 0, max_child_time = 0;

    start = clock(); // Starts the stopwatch

    // Creating pipes
    int fd[num_processes][2];
    for (int i = 0; i < num_processes; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Forking child processes
    int rows_per_process = N / num_processes;
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            close(fd[i][READ_END]); // Close unused read end

            int start_row = i * rows_per_process;
            int end_row = (i + 1) * rows_per_process;
            if (i == num_processes - 1) {
                end_row = N; // Last process handles remaining rows
            }

            clock_t child_start = clock(); // Start child's stopwatch

            // Perform multiplication for the assigned rows
            multiplyMatrices(A, B, C, start_row, end_row, N);

            clock_t child_end = clock(); // Stop child's stopwatch
            double child_time = ((double)(child_end - child_start)) / CLOCKS_PER_SEC;

            // Write the result back to the parent
            write(fd[i][WRITE_END], &start_row, sizeof(int));
            write(fd[i][WRITE_END], &end_row, sizeof(int));
            write(fd[i][WRITE_END], &child_time, sizeof(double));
            for (int j = start_row; j < end_row; j++) {
                write(fd[i][WRITE_END], C[j], N * sizeof(int));
            }

            close(fd[i][WRITE_END]);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process
    for (int i = 0; i < num_processes; i++) {
        close(fd[i][WRITE_END]); // Close unused write ends
    }

    // Read results from child processes and construct the final matrix
    for (int i = 0; i < num_processes; i++) {
        int start_row, end_row;
        double child_time;
        read(fd[i][READ_END], &start_row, sizeof(int));
        read(fd[i][READ_END], &end_row, sizeof(int));
        read(fd[i][READ_END], &child_time, sizeof(double));
        if (child_time > max_child_time) {
            max_child_time = child_time;
        }
        for (int j = start_row; j < end_row; j++) {
            read(fd[i][READ_END], C[j], N * sizeof(int));
        }
        close(fd[i][READ_END]);
    }

    end = clock(); // Stop the stopwatch
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate total CPU time

    // Add maximum child time to total CPU time
    cpu_time_used += max_child_time;

    // printf("N = %d, Total CPU Time = %f\n", N, cpu_time_used);
    printf("%d, %d, %f\n", num_processes, N, cpu_time_used);

    // Deallocate memory
    freeMatrices(A, B, C, N);

    return 0;
}
