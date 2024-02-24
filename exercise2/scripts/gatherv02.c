#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// compile with: mpicc gatherv02.c -o gatherv02
// run with: mpirun -np 4 ./gatherv02

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf_size = rank + 1;  // Size of the array to be sent
    int sendbuf[sendbuf_size];    // Array to be sent by each process
    int recvbuf[size * (size + 1) / 2];  // Array to store gathered data at root

    int counts[size];        // Array to store the count of elements each process sends
    int displs[size];        // Array to store the displacements for each process

    // Initialize the sendbuf array with consecutive integers
    for (int i = 0; i < sendbuf_size; i++) {
        sendbuf[i] = (rank + 1) * 10 + i;  // Example values based on rank and index
    }

    // Print the elements of each rank before the MPI_Gatherv operation
    printf("Rank %d: Sending array: ", rank);
    for (int i = 0; i < sendbuf_size; i++) {
        printf("%d ", sendbuf[i]);
    }
    printf("\n");

    // Setting counts and displs arrays for MPI_Gatherv
    for (int i = 0; i < size; i++) {
        counts[i] = i + 1;      // Count is the size of the array to be sent
        displs[i] = (i * (i + 1)) / 2;  // Displacement is the cumulative sum of sizes
    }

    // Gather data from all processes to the root (rank 0)
    MPI_Gatherv(sendbuf, // The send buffer
      sendbuf_size, // The number of elements to send to each process
      MPI_INT, // The data type of the send buffer
      recvbuf, // The receive buffer for this process
      counts, // The count of elements each process sends
      displs, // The displacements for each process
      MPI_INT, // The data type of the receive buffer
      0, // The rank of the root process
      MPI_COMM_WORLD); // The communicator

    // Print the gathered data at the root process
    if (rank == 0) {
        printf("\nGathered data at root:\n");
        for (int i = 0; i < size * (size + 1) / 2; i++) {
            printf("%d ", recvbuf[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
