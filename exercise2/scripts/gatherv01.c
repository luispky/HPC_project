#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// compile with: mpicc gatherv01.c -o gatherv01
// run with: mpirun -np 4 ./gatherv01

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf = rank + 1;  // Each process sends its rank + 1
    int recvbuf[size];      // Array to store gathered data at root

    int counts[size];        // Array to store the count of elements each process sends
    int displs[size];        // Array to store the displacements for each process

    // Print the elements of each rank before the MPI_Gatherv operation
    printf("Rank %d: Sending %d\n", rank, sendbuf);

    // Setting counts and displs arrays for MPI_Gatherv
    for (int i = 0; i < size; i++) {
        counts[i] = 1;          // Each process sends one integer
        displs[i] = i;          // Displacement is the rank of the process
    }

    // Gather data from all processes to the root (rank 0)
    MPI_Gatherv(&sendbuf, 1, MPI_INT, recvbuf, counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the gathered data at the root process
    if (rank == 0) {
        printf("\nGathered data at root:\n");
        for (int i = 0; i < size; i++) {
            printf("%d ", recvbuf[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
