#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_PROCESSES 0

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

void MultiplyMatrices(int **A, int **B, int **C, int N, double *totalMaxExecutionTime, int *numberOfProccess) {
    int shmid = shmget(IPC_PRIVATE, sizeof(int) * N * N, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error en shmget");
        exit(1);
    }

    int *resultShared = (int *)shmat(shmid, NULL, 0);
    if ((intptr_t)resultShared == -1) {
        perror("Error en shmat");
        exit(1);
    }

    // Calcular la cantidad de procesos hijos necesarios
    int numProcesses = (N < *numberOfProccess) ? N : *numberOfProccess;

    // Calcular el tamaño del bloque para cada proceso hijo
    int blockSize = N / numProcesses;

    // Crear memoria compartida para totalMaxExecutionTimeTemporal
    int totalMaxExecutionTimeShmid = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0666);
    if (totalMaxExecutionTimeShmid == -1) {
        perror("Error en shmget para totalMaxExecutionTimeTemporal");
        exit(1);
    }

    double *totalMaxExecutionTimeTemporal = (double *)shmat(totalMaxExecutionTimeShmid, NULL, 0);
    if ((intptr_t)totalMaxExecutionTimeTemporal == -1) {
        perror("Error en shmat para totalMaxExecutionTimeTemporal");
        exit(1);
    }

    // Inicializar totalMaxExecutionTimeTemporal en 0.0
    *totalMaxExecutionTimeTemporal = 0.0;

    // Proceso para la multiplicación de matrices
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Proceso hijo
            clock_t start, end;
            start = clock();

            // Calcular los índices de inicio y fin para este proceso hijo
            int startRow = i * blockSize;
            int endRow = (i == numProcesses - 1) ? N : startRow + blockSize;

            // Multiplicación de matrices para el bloque asignado al hijo
            for (int row = startRow; row < endRow; row++) {
                for (int col = 0; col < N; col++) {
                    resultShared[row * N + col] = 0;
                    for (int k = 0; k < N; k++) {
                        resultShared[row * N + col] += A[row][k] * B[k][col];
                    }
                }
            }

            end = clock();
            double executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;

            // Comparar el tiempo actual con el máximo y actualizar si es necesario
            if (executionTime > *totalMaxExecutionTimeTemporal) {
                *totalMaxExecutionTimeTemporal = executionTime;
            }

            exit(0);
        } else if (pid < 0) {
            perror("Error en fork");
            exit(1);
        }

        // Verificar si se deben recolectar tiempos máximos
        if ((i + 1) % *numberOfProccess == 0 || i == numProcesses - 1) {
            // Esperar a que todos los procesos hijos del grupo actual terminen
            for (int j = 0; j < *numberOfProccess && (i - j) >= 0; j++) {
                wait(NULL);
            }

             // Sumar el tiempo máximo de totalMaxExecutionTimeTemporal al total
             *totalMaxExecutionTime += *totalMaxExecutionTimeTemporal;

            // Reiniciar la variable para el próximo grupo
            *totalMaxExecutionTimeTemporal = 0.0;
        }
    }

    // Copiar el resultado desde la memoria compartida a la matriz resultante
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = resultShared[i * N + j];
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


void getCommands(int argc, char *argv[], int *N, int *verbose, int *numberOfProccess){

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1; // Enable verbose output
        } else if (i == 1) {
            *N = atoi(argv[i]); // First positional argument is matrix size
        } else if (i == 2) {
            *numberOfProccess = atoi(argv[i]); // First positional argument is matrix size
        }
    }

    if (*N == 0) {
        printf("Usage: %s <N> [-v|--verbose]\n", argv[0]);
        exit(1);
    }

    if (*numberOfProccess == 0) {
        printf("Usage: %s <N> <numberOfProccess> [-v|--verbose]\n", argv[0]);
        exit(1);
    }

}


int main(int argc, char *argv[]) {

    int N = 0, verbose = 0, numberOfProccess= 0;

    // Process command line arguments
    getCommands(argc, argv, &N, &verbose, &numberOfProccess);

    // Memory allocation
    int **A, **B, **C;
    allocateMatrices(&A, &B, &C, N);

    // Filling matrices
    fillMatrices(A, B, C, N);

    // Variables para medir el tiempo total y el tiempo máximo de ejecución
    clock_t startTotal, endTotal;
    double totalCpuTimeUsed, totalMaxExecutionTime = 0.0, totalMainThread = 0.0;

    startTotal = clock(); // Inicia el cronómetro total

    // Llamada a MultiplyMatrices con la referencia a totalMaxExecutionTime
    MultiplyMatrices(A, B, C, N, &totalMaxExecutionTime, &numberOfProccess);

    endTotal = clock(); // Detiene el cronómetro total

    totalMainThread = ((double) (endTotal - startTotal)) / CLOCKS_PER_SEC;

    totalCpuTimeUsed = totalMaxExecutionTime + totalMainThread;

    if (verbose) {
        printMatrices(A, B, C, N);
        // Imprimir resultados

        printf("Total CPU Time: %f\n", totalCpuTimeUsed);
        printf("Total Max Execution Time: %f\n", totalMaxExecutionTime);
    } else {
        //printf("%d, %f, %f, %f\n", N, totalMainThread, totalMaxExecutionTime, totalCpuTimeUsed);
        printf("%d, %d, %f\n", numberOfProccess, N, totalCpuTimeUsed);
    }

    // Liberar memoria
    freeMatrices(A, B, C, N);

    return 0;
}