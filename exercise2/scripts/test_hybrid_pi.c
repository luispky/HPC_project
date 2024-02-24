#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// compile with: mpicc -fopenmp test_hybrid_pi.c -o test_hybrid_pi -lm
// run with: mpirun -n 4 ./test_hybrid_pi 1000000 12345

#define TOTAL_POINTS 1000000

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

    omp_set_num_threads(4);

    // Check if enough command line arguments are provided
    if (argc < 3) {
        printf("Usage: %s <total_points> <random_seed>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }

    // Extract command line arguments
    int total_points = atoi(argv[1]);
    int random_seed = atoi(argv[2]);

    // Seed the random number generator based on the rank and provided seed
    srand(rank * random_seed);

    // Local points for Monte Carlo simulation
    int local_points = total_points / size;
    int points_inside_circle = 0;

    // Perform Monte Carlo simulation in parallel using OpenMP
    #pragma omp parallel for reduction(+:points_inside_circle)
    for (int i = 0; i < local_points; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        double distance = sqrt(x * x + y * y);

        if (distance <= 1.0) {
            points_inside_circle++;
        }
    }

    // Sum up the points inside the circle across all MPI processes
    int total_inside_circle;
    MPI_Reduce(&points_inside_circle, &total_inside_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Calculate the overall estimate of π
    double pi_estimate = 4.0 * total_inside_circle / total_points;

    // Print the result (only on rank 0)
    if (rank == 0) {
        printf("Estimate of π: %lf\n", pi_estimate);
    }

    MPI_Finalize();

    return 0;
}
