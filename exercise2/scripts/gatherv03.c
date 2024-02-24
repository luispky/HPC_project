#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// compile: mpicc -o gatherv03 gatherv03.c
// run: mpirun -np 4 ./gatherv03

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Flatten the arrays before sending
    int *flattened_sendbuf = (int *)malloc(4 * (rank + 1) * sizeof(int));
    for (int i = 0; i <= rank; i++) {
        for (int j = 0; j < 4; j++) {
            flattened_sendbuf[i * 4 + j] = (i + 1) * 10 + j + 1;
        }
    }

    // Print elements each rank sends
    printf("Rank %d: Sending array of arrays:\n", rank);
    for (int i = 0; i <= rank; i++) {
        for (int j = 0; j < 4; j++) {
            printf("  %d ", flattened_sendbuf[i * 4 + j]);
        }
        printf("\n");
    }

    // Determine the total number of elements in the flattened_sendbuf array
    int total_elements = 4 * size * (size + 1) / 2;

    // Prepare arrays for MPI_Gatherv
    int *recvbuf = NULL;
    int *recvcounts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        recvcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));

        // Initialize recvcounts and displs arrays
        for (int i = 0; i < size; i++) {
            recvcounts[i] = 4 * (i + 1);
            displs[i] = (i == 0) ? 0 : displs[i - 1] + recvcounts[i - 1];
            // Print the values of recvcounts and displs
            printf("Rank %d: recvcounts[%d] = %d, displs[%d] = %d\n", rank, i, recvcounts[i], i, displs[i]);
        }

        // Allocate memory for recvbuf array
        recvbuf = (int *)malloc(total_elements * sizeof(int));
    }

    // Barrier to ensure all ranks have completed the above code before proceeding
    MPI_Barrier(MPI_COMM_WORLD);

    // Gather flattened arrays at root
    MPI_Gatherv(flattened_sendbuf,
        4 * (rank + 1),
        MPI_INT,
        recvbuf,
        recvcounts,
        displs,
        MPI_INT,
        0,
        MPI_COMM_WORLD);

    // Print gathered data at root
    if (rank == 0) {
        printf("\nGathered data at root:\n");
        for (int i = 0; i < total_elements; i++) {
            printf("  %d ", recvbuf[i]);
        }
        printf("\n");
    }

    // Free the allocated memory
    free(flattened_sendbuf);

    if (rank == 0) {
        free(recvbuf);
        free(recvcounts);
        free(displs);
    }

    MPI_Finalize();

    return 0;
}
