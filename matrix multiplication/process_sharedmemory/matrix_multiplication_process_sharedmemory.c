#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define N 3

// Función para imprimir una matriz
void printMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int matrixA[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int matrixB[N][N] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int resultMatrix[N][N];

    // Crear memoria compartida para la matriz resultante
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

    // Proceso para la multiplicación de matrices
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Proceso hijo
            for (int j = 0; j < N; j++) {
                resultShared[i * N + j] = 0;
                for (int k = 0; k < N; k++) {
                    resultShared[i * N + j] += matrixA[i][k] * matrixB[k][j];
                }
            }
            exit(0);
        } else if (pid < 0) {
            perror("Error en fork");
            exit(1);
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    // Copiar el resultado desde la memoria compartida a la matriz resultante
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            resultMatrix[i][j] = resultShared[i * N + j];
        }
    }

    // Imprimir matrices y el resultado
    printf("Matriz A:\n");
    printMatrix(matrixA);

    printf("\nMatriz B:\n");
    printMatrix(matrixB);

    printf("\nMatriz Resultante:\n");
    printMatrix(resultMatrix);

    // Liberar la memoria compartida
    shmdt(resultShared);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}