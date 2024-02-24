#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// to compile: mpicc -fopenmp test_hybrid.c -o test_hybrid
// to run: mpirun -n 4 ./test_hybrid

#define N 1000
#define CHUNK_SIZE 100

void matrix_multiply(int start_row, int end_row, int** matrix_A, int** matrix_B, int** result) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
            for (int k = 0; k < N; k++) {
                result[i][j] += matrix_A[i][k] * matrix_B[k][j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int mpi_provided_thread_level;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &mpi_provided_thread_level);

    if (mpi_provided_thread_level < MPI_THREAD_FUNNELED) {
        printf("A problem arises when asking for MPI_THREAD_FUNNELED level\n");
        MPI_Finalize();
        exit(1);
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize matrices A, B, and result
    int** matrix_A = (int**)malloc(N * sizeof(int*));
    int** matrix_B = (int**)malloc(N * sizeof(int*));
    int** result = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; i++) {
        matrix_A[i] = (int*)malloc(N * sizeof(int));
        matrix_B[i] = (int*)malloc(N * sizeof(int));
        result[i] = (int*)malloc(N * sizeof(int));

        for (int j = 0; j < N; j++) {
            matrix_A[i][j] = i + j;
            matrix_B[i][j] = i - j;
            result[i][j] = 0;
        }
    }

    // Distribute work using MPI
    int start_row = rank * CHUNK_SIZE;
    int end_row = (rank + 1) * CHUNK_SIZE;

    if (end_row > N) {
        end_row = N;
    }

    // Perform matrix multiplication in parallel using OpenMP
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        int thread_start_row = i * CHUNK_SIZE;
        int thread_end_row = (i + 1) * CHUNK_SIZE;

        if (thread_end_row > N) {
            thread_end_row = N;
        }

        matrix_multiply(thread_start_row, thread_end_row, matrix_A, matrix_B, result);
    }

    // Gather results back using MPI
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(result[start_row], CHUNK_SIZE * N, MPI_INT, result[0], CHUNK_SIZE * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the result (only on rank 0)
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d\t", result[i][j]);
            }
            printf("\n");
        }
    }

    // Cleanup
    for (int i = 0; i < N; i++) {
        free(matrix_A[i]);
        free(matrix_B[i]);
        free(result[i]);
    }
    free(matrix_A);
    free(matrix_B);
    free(result);

    MPI_Finalize();

    return 0;
}
