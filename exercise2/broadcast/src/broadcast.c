#include <mpi.h>

/* Syntax of MPI broadcast
int MPI_Bcast(void *buffer,
              int count,
              MPI_Datatype datatype,
              int root,
              MPI_Comm comm);

buffer: Address of data to be broadcast (sender) or receive buffer (receiver).
count: Number of data elements in the buffer.
datatype: MPI data type of each element in the buffer.
root: Rank of the process sending the data (source of broadcast).
comm: Communicator over which the broadcast is performed.
*/


/*
  * This function implements a broadcast operation using a flat tree.
  * The root process sends the data to all other processes.
  * The root process can be different from the process 0.
  * The root process should not send data to itself.
*/  
int Bcast_flat_tree(void* buffer, 
                    int count,
                    MPI_Datatype datatype,
                    int root,
                    MPI_Comm comm) {
  // Handle errors in the input arguments
  if (count < 0) { // If the number of data elements in the buffer is negative, return an error.
    return MPI_ERR_COUNT;
  } else if (!buffer) { // If the buffer is NULL, return an error.
    return MPI_ERR_BUFFER;
  } else if (!comm) { // If the communicator is NULL, return an error.
    return MPI_ERR_COMM;
  } else if (MPI_Type_commit(&datatype)) { // If the MPI data type of each element in the buffer is not committed, return an error.
    return MPI_ERR_TYPE;
  }
  
  int rank, nproc;
  // Copy the number of processes and the rank of the calling process 
  // from the global communicator.
  MPI_Comm_size(comm, &nproc);
  MPI_Comm_rank(comm, &rank);

  if (nproc == 1) { // If there is only one process, there is no need to send data.
    return MPI_SUCCESS;
  }
  // Handle more errors in the input arguments
  if (root < 0 || root >= nproc) { 
    return MPI_ERR_ROOT;
  }

  if (rank != root) { // The processes other than the root process are ready to receive the data
    MPI_Recv(buffer,    // Address of the receive buffer to store the data
            count,      // Number of data elements in the buffer
            datatype,   // MPI data type of each element in the buffer
            root,       // Rank of the process sending the data
            0,          // Tag
            comm,       // Communicator over which the broadcast is performed
            MPI_STATUS_IGNORE); // Status object

  } else { // The root process sends the data to all other processes
    for (unsigned int other_rank = 0; i < nproc; other_rank++) {
      if (other_rank != rank) { // The root process can be different from the process 0
                                // and the root process should not send data to itself.
        MPI_Send(buffer,      // Address of the data to be sent
                count,        // Number of data elements in the buffer
                datatype,     // MPI data type of each element in the buffer
                other_rank,   // Rank of the process receiving the data
                0,           // Tag
                comm);       // Communicator over which the broadcast is performed
      }
    }
  }

  return MPI_SUCCESS;
}

/*
  * This function implements a broadcast operation using a binomial tree. 
  * Reference: https://en.wikipedia.org/wiki/Broadcast_(parallel_pattern)
*/

void Bcast_binomial_tree(void* buffer, 
                        int count,
                        MPI_Datatype datatype,
                        int root,
                        MPI_Comm comm) {
  // Handle errors in the input arguments
  if (count < 0) { // If the number of data elements in the buffer is negative, return an error.
    return MPI_ERR_COUNT;
  } else if (!buffer) { // If the buffer is NULL, return an error.
    return MPI_ERR_BUFFER;
  } else if (!comm) { // If the communicator is NULL, return an error.
    return MPI_ERR_COMM;
  } else if (MPI_Type_commit(&datatype)) { // If the MPI data type of each element in the buffer is not committed, return an error.
    return MPI_ERR_TYPE;
  }
  
  int rank, nproc;
  // Copy the number of processes and the rank of the calling process 
  // from the global communicator.
  MPI_Comm_size(comm, &nproc);
  MPI_Comm_rank(comm, &rank);

  if (nproc == 1) { // If there is only one process, there is no need to send data.
    return MPI_SUCCESS;
  }
  // Handle more errors in the input arguments
  if (root < 0 || root >= nproc) { 
    return MPI_ERR_ROOT;
  }

  // Reorganize the ranks so that the root process is rank 0
  int new_rank = (rank - root + nproc) % nproc;

  if (new_rank != root)
    MPI_Recv(buffer, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);

  unsigned int num_of_iterations = ceil(log2(nproc));
  for (unsigned int i = num_of_iterations - 1; i >= 0; i--) {
    if (new_rank % (1 << (i + 1)) == 0 && new_rank + (1 << i) <= nproc) { // 1 << i is 2^i
      MPI_Send(buffer, count, datatype, new_rank + (1 << i), 0, comm);
    }
  }

  return MPI_SUCCESS;
}

