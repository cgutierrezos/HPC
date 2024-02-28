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
            int sum = 0;

            clock_t child_start, child_end;
            child_start = clock(); // Start the stopwatch

            // Perform multiplication for the row
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                // Write the result to pipe
                if (write(fd[i][WRITE_END], &sum, sizeof(int)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
                sum = 0;
            }

            close(fd[i][WRITE_END]); // Close write end after writing

            child_end = clock(); // Stop the stopwatch
            double child_cpu_time_used = ((double)(child_end - child_start)) / CLOCKS_PER_SEC;
            printf("Child %d CPU time: %f\n", i, child_cpu_time_used);

            // Write child's CPU time to pipe
            if (write(fd[i][WRITE_END], &child_cpu_time_used, sizeof(double)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
    }

    // Parent process
    for (int i = 0; i < N; i++) {
        close(fd[i][WRITE_END]); // Close write end in parent
    }

    // Wait for all child processes to finish
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Reading results from child processes and updating C matrix
    int sum;
    for (int i = 0; i < N; i++) {
        close(fd[i][WRITE_END]); // Close write end after reading

        // Read child's CPU time from pipe
        if (read(fd[i][READ_END], &child_times[i], sizeof(double)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        while (read(fd[i][READ_END], &sum, sizeof(int)) != -1) {
            C[i][i] = sum;
        }

        close(fd[i][READ_END]); // Close read end after reading
    }

    end = clock(); // Stops the stopwatch

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    double max_child_time = 0;
    for (int i = 0; i < N; i++) {
        if (child_times[i] > max_child_time) {
            max_child_time = child_times[i];
        }
    }

    double total_time = cpu_time_used + max_child_time;

    if (verbose) {
        printMatrices(A, B, C, N);
        // Printing results
        printf("Time Clock to process: %f\n", cpu_time_used);
        printf("Max Child Time: %f\n", max_child_time);
        printf("Total Time: %f\n", total_time);
    } else {
        printf("%d, %f\n", N, total_time);
    }

    // Freeing memory
    freeMatrices(A, B, C, N);

    return 0;
}
