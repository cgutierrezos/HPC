#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

void printMatrix(int **matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
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

void MultiplyMatrices(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
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

void printMatrices(int **A, int **B, int **C, int N) {
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

void getCommands(int argc, char *argv[], int *N, int *verbose) {
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
    double cpu_time_used = 0;

    start = clock(); // Starts the stopwatch

    int fd[N][2]; // Array of pipes for communication with child processes
    double child_times[N];

    // Creating pipes
    for (int i = 0; i < N; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Forking child processes
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            close(fd[i][READ_END]); // Close unused read end

            clock_t child_start, child_end;
            child_start = clock(); // Start the stopwatch

            // Perform multiplication for the row
            for (int j = 0; j < N; j++) {
                int sum = 0;
                for (int k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                write(fd[i][WRITE_END], &sum, sizeof(int)); // Write the sum to the pipe
            }

            child_end = clock(); // Stop the stopwatch
            close(fd[i][WRITE_END]); // Close the write end after use

            double child_cpu_time_used = ((double)(child_end - child_start)) / CLOCKS_PER_SEC;
            write(fd[i][WRITE_END], &child_cpu_time_used, sizeof(double)); // Write child's CPU time to the pipe
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process
    for (int i = 0; i < N; i++) {
        close(fd[i][WRITE_END]); // Close unused write ends
    }

    // Read from pipes and update C matrix
    for (int i = 0; i < N; i++) {
        int sum;
        while (read(fd[i][READ_END], &sum, sizeof(int)) > 0) {
            C[i][sum % N] = sum;
        }
    }

    // Read child CPU times from pipes
    for (int i = 0; i < N; i++) {
        double child_cpu_time_used;
        read(fd[i][READ_END], &child_cpu_time_used, sizeof(double)); // Read child's CPU time from the pipe
        child_times[i] = child_cpu_time_used;
    }

    // Close all pipe file descriptors
    for (int i = 0; i < N; i++) {
        close(fd[i][READ_END]);
    }

    // Determine which child process took the most time
    double max_child_time = 0;
    for (int i = 0; i < N; i++) {
        if (child_times[i] > max_child_time) {
            max_child_time = child_times[i];
        }
    }

    end = clock(); // Stop the stopwatch
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate total CPU time for parent

    // Print the result
    printf("N = %d, Total CPU Time = %f\n", N, cpu_time_used + max_child_time);

    // Deallocate memory
    freeMatrices(A, B, C, N);

    return 0;
}
