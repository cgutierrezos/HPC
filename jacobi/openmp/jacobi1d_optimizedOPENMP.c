#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

/* --
 * Do nsweeps sweeps of Jacobi iteration on a 1D Poisson problem
 * 
 *    -u'' = f
 *
 * discretized by n+1 equally spaced mesh points on [0,1].
 * u is subject to Dirichlet boundary conditions specified in
 * the u[0] and u[n] entries of the initial vector.
 */
void jacobi(int nsweeps, int n, double* u, double* f) {
    double h = 1.0 / n;
    double h2 = h * h;
    double* utmp = (double*) malloc((n + 1) * sizeof(double));
    memcpy(utmp, u, (n + 1) * sizeof(double)); // Inicializa utmp con los valores actuales de u

    for (int sweep = 0; sweep < nsweeps; sweep++) {
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            utmp[i] = (u[i - 1] + u[i + 1] + h2 * f[i]) / 2;
        }
        double* temp = u;
        u = utmp;
        utmp = temp;
    }

    memcpy(u, utmp, (n + 1) * sizeof(double));
    free(utmp);
}

void write_solution(int n, double* u, const char* fname) {
    int i;
    double h = 1.0 / n;
    FILE* fp = fopen(fname, "w+");
    for (i = 0; i <= n; ++i)
        fprintf(fp, "%g %g\n", i*h, u[i]);
    fclose(fp);
}

void getCommands(int argc, char *argv[], int *nsteps, int *N, int *verbose, int *numThreads, char **fname) {
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            *verbose = 1; // Enable verbose output
        } else if (i == 1) {
            *N = atoi(argv[i]); // First positional argument is matrix size
        } else if (i == 2) {
            *nsteps = atoi(argv[i]); // Second positional argument is number of steps
        } else if (i == 3) {
            *numThreads = atoi(argv[i]); // Third positional argument is number of threads
        } else if (i == 4) {
            *fname = argv[i]; // Fourth positional argument is file name
        }
    }

    if (*fname == NULL)
        *fname = "results.out";

    if (*N == 0 || *nsteps == 0 || *numThreads == 0) {
        printf("Usage: %s <N> <nsteps> <numThreads> [fname=results.out] [-v|--verbose]\n", argv[0]);
        exit(1);
    }
}

int main(int argc, char** argv) {
    int n, nsteps, verbose, numThreads;
    double* u;
    double* f;
    double h;
    char* fname;

    getCommands(argc, argv, &nsteps, &n, &verbose, &numThreads, &fname);

    u = (double*) malloc((n + 1) * sizeof(double));
    f = (double*) malloc((n + 1) * sizeof(double));

    h = 1.0 / n;

    for (int i = 0; i <= n; ++i) {
        u[i] = 0.0;
        f[i] = i * h;
    }

    omp_set_num_threads(numThreads);

    double start_time = omp_get_wtime();

    jacobi(nsteps, n, u, f);

    double end_time = omp_get_wtime();
    double total_time = end_time - start_time;

    /* Run the solver */
    printf("%d, %d, %d, %f\n", numThreads, n, nsteps, total_time);

    /* Write the results */
    if (fname) {
        write_solution(n, u, fname);
    }

    free(u);
    free(f);

    return 0;
}