#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

// compile with: mpicc -fopenmp test_hybrid_pi2.c -o test_hybrid_pi2 -lm
// run with: mpirun -n 4 ./test_hybrid_pi2 1000000 12345

#define TOTAL_POINTS 1000000

// Perform Monte Carlo simulation in parallel using OpenMP
int MonteCarlosimulation(int start_index, int end_index) {
    int points_inside_circle = 0;
    #pragma omp parallel for reduction(+:points_inside_circle)
    for (int i = start_index; i < end_index; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        double distance = sqrt(x * x + y * y);

        if (distance <= 1.0) {
            points_inside_circle++;
        }
    }
    return points_inside_circle;
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

    // Set the number of OpenMP threads
    omp_set_dynamic(0); // Disable dynamic teams
    omp_set_num_threads(11);

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
    int local_points = TOTAL_POINTS / size;

    // Distribute work among MPI processes in a FCFS manner
    int start_index = rank * local_points;
    int end_index = (rank + 1) * local_points;

    int points_inside_circle = MonteCarlosimulation(start_index, end_index);

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
