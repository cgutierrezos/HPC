#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "timing.h"


/* --
 * Do nsweeps sweeps of Jacobi iteration on a 1D Poisson problem
 * 
 *    -u'' = f
 *
 * discretized by n+1 equally spaced mesh points on [0,1].
 * u is subject to Dirichlet boundary conditions specified in
 * the u[0] and u[n] entries of the initial vector.
 */
void jacobi(int nsweeps, int n, double* u, double* f, int start, int end, int BLOCK_SIZE) {
    double h = 1.0 / n;
    double h2 = h * h;
    double* utmp = (double*) malloc((n + 1) * sizeof(double));
    memcpy(utmp, u, (n + 1) * sizeof(double)); // Inicializa utmp con los valores actuales de u

    for (int sweep = 0; sweep < nsweeps; sweep++) {
        for (int block_start = start; block_start < end; block_start += BLOCK_SIZE) {
            int block_end = block_start + BLOCK_SIZE < end ? block_start + BLOCK_SIZE : end;
            for (int i = block_start; i < block_end; i++) {
                // Realiza el cálculo de Jacobi aquí, teniendo en cuenta los bordes del bloque
                utmp[i] = (u[i - 1] + u[i + 1] + h2 * f[i]) / 2;
            }
        }
        
        double* temp = u;
        u = utmp;
        utmp = temp;
    }

    memcpy(u, utmp, (n + 1) * sizeof(double));

    // Final: Posiblemente sincronizar/actualizar el vector 'u' principal en el proceso padre

    free(utmp);
}


void write_solution(int n, double* u, const char* fname)
{
    int i;
    double h = 1.0 / n;
    FILE* fp = fopen(fname, "w+");
    for (i = 0; i <= n; ++i)
        fprintf(fp, "%g %g\n", i*h, u[i]);
    fclose(fp);
}

void getCommands(int argc, char *argv[], int *nsteps, int *N, int *verbose, int *numProcess, int *BLOCK_SIZE, char **fname){

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {


        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1; // Enable verbose output
        } else if (i == 1) {
            *N = atoi(argv[i]); // First positional argument is matrix size
        } else if (i == 2) {
            *nsteps = atoi(argv[i]); // Second positional argument is matrix size
        } else if (i == 3) {
            *numProcess = atoi(argv[i]); // Third positional argument is matrix size
        } else if (i == 4) {
            *BLOCK_SIZE = atoi(argv[i]); // Third positional argument is matrix size
        } else if (i == 5) {
            *fname = argv[i]; // Fourth positional argument is matrix size
        }
    }

    if (*BLOCK_SIZE == 0)
        *BLOCK_SIZE = 10240;
    

    if (*fname == NULL)
        *fname = "results.out";

    if (*N == 0 || *nsteps == 0 || *numProcess == 0) {
        printf("Usage: %s <N> <nsteps> <numProcess> [BLOCK_SIZE=1024] [fname=results.out] [-v|--verbose]\n", argv[0]);
        exit(1);
    }


}


int main(int argc, char** argv)
{
    int i, n, nsteps, verbose, numProcess, segment_size, BLOCK_SIZE;
    double* u;
    double* f;
    double h;
    char* fname;

    getCommands(argc, argv, &nsteps, &n, &verbose, &numProcess, &BLOCK_SIZE, &fname);

    segment_size  = (n + 1) / numProcess;
    h = 1.0/n;

    int shm_id_time = shmget(IPC_PRIVATE, sizeof(double), IPC_CREAT | 0666);
    double* max_time = (double*) shmat(shm_id_time, NULL, 0);
    *max_time = 0.0; // Inicializar el tiempo máximo a 0

    int shm_id_u = shmget(IPC_PRIVATE, (n+1) * sizeof(double), IPC_CREAT | 0666);
    u = (double*) shmat(shm_id_u, NULL, 0);

    int shm_id_f = shmget(IPC_PRIVATE, (n+1) * sizeof(double), IPC_CREAT | 0666);
    f = (double*) shmat(shm_id_f, NULL, 0);
    
    for (int i = 0; i <= n; ++i) {
        u[i] = 0.0;
        f[i] = i * h;
    }

    clock_t start_main = clock();

    for (int i = 0; i < numProcess; i++) {

        int start = i * segment_size;
        int end = (i == numProcess - 1) ? n : (start + segment_size - 1);

        pid_t pid = fork();

        if (pid == 0) { // Proceso hijo

            clock_t start_child = clock();
            jacobi(nsteps, n, u, f, start, end, BLOCK_SIZE);
            clock_t end_child = clock();
            double duration = (double)(end_child - start_child) / CLOCKS_PER_SEC;

            // Actualizar el tiempo máximo si es necesario
            if (duration > *max_time) {
                *max_time = duration;
            }

            exit(0);

        } else if (pid < 0) {

            perror("Fork failed");
            exit(1);

        }
    }

    for (int i = 0; i < numProcess; i++) {
        wait(NULL);
    }

    clock_t end_main = clock();
    double duration_main = (double)(end_main - start_main) / CLOCKS_PER_SEC;
    double total_time = *max_time + duration_main;

    /* Run the solver */    
    printf("%d, %d, %d, %f\n", numProcess, n, nsteps, total_time);

    /* Write the results */
    if (fname) {
        write_solution(n, u, fname);
    }

    shmdt(u);
    shmdt(f);

    // Marcar la memoria compartida para su eliminación
    shmctl(shm_id_u, IPC_RMID, NULL);
    shmctl(shm_id_f, IPC_RMID, NULL);

    return 0;
}
