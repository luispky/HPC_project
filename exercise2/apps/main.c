/*
* Mandelbrot Set Generation using MPI and OpenMP
* Author: Luis Fernando Palacios Flores
* Date: February 2024
*/

#include <stdio.h> // For printf
#include <stdlib.h> // For atoi, atof
#include <mpi.h> // For MPI functions
#include <omp.h> // For OpenMP functions
#include <time.h> // For clock_gettime
#include "mandelbrot.h" // For the mandelbrot function

// Define the default values for the number of threads, the dimensions of the image,
// the region of the complex plane, and the maximum number of iterations
#define N_THREADS 12
#define DIM_X 1000
#define DIM_Y 1000
#define X_L -2.0
#define Y_L -1.0
#define X_R 0.5
#define Y_R 1.0
#define MAX_ITERATIONS 65535
#define WRITE_PGM 0

int main(int argc, char** argv) {

  // Initialize MPI
  int mpi_provided_thread_level;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &mpi_provided_thread_level);

  if (mpi_provided_thread_level < MPI_THREAD_FUNNELED) {
      printf("A problem arose when asking for MPI_THREAD_FUNNELED level\n");
      MPI_Finalize();
      exit(1);
  }

  // Get the rank and size in the original communicator
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Get the number of threads for OpenMP,
  // the dimensions of the image,
  // the region of the complex plane,
  // and the maximum number of iterations 
  int N_threads = (argc > 1) ? atoi(*(argv+1)) : N_THREADS;
  int n_x = (argc > 2) ? atoi(*(argv+2)) : DIM_X;
  int n_y = (argc > 3) ? atoi(*(argv+3)) : DIM_Y;
  double x_L = (argc > 4) ? atof(*(argv+4)) : X_L;
  double y_L = (argc > 5) ? atof(*(argv+5)) : Y_L;
  double x_R = (argc > 6) ? atof(*(argv+6)) : X_R;
  double y_R = (argc > 7) ? atof(*(argv+7)) : Y_R;
  int I_max = (argc > 8) ? atoi(*(argv+8)) : MAX_ITERATIONS;

  // Print the values of the variables
  if (rank == 0) {
    printf("Number of MPI processes: %d\n", size);
    printf("Number of OMP threads: %d\n", N_threads); 
    printf("Image dimensions: %d x %d\n", n_x, n_y);
    printf("Complex plane region: [%.2f, %.2f] x [%.2f, %.2f]\n", x_L, x_R, y_L, y_R);
    printf("Maximum number of iterations: %d\n", I_max);
  }

  // Set the number of threads for OpenMP
  omp_set_dynamic(0); // Disable dynamic teams
  omp_set_num_threads(N_threads); // Use N_threads threads

  // Variables to record the start and end time of the parallel computation
  double start_time_MPI;
  double end_time_MPI;
  double start_time_OMP;
  double end_time_OMP;
  double start_time;
  double end_time;

  //===================================TIMING PARALLEL EXECUTION ==================================

  // Record the start time for the parallel computation using MPI
  start_time_MPI = MPI_Wtime();

  // ----Variables to perform the parallel computation----
  // Calculate the number of rows to be computed by each process
  int local_ny = n_y / size;
  int reminder = n_y % size;
  int local_ny_start = local_ny * rank + ((rank < reminder) ? rank : reminder);
  int local_ny_stop = local_ny_start + local_ny + ((rank < reminder) ? 1 : 0);
  // Calculate the size of the send buffer for each process
  int sendbuf_size_process = (local_ny_stop - local_ny_start) * n_x;
  // Calculate the displacement for each process
  int displ_process = local_ny_start * n_x;

  // ----Variables to collect the data from all processes----
  // Allocate memory for the send and receive buffers
  void *img_sendbuf = NULL;
  void *img_recvbuf = NULL;
  // If the maximum number of iterations is less than 256, use char data type for the image
  if (I_max <  256) { // Touch-first policy for convenient memory allocation
    // img_sendbuf = (char *)calloc(sendbuf_size_process * sizeof(char));   
    img_sendbuf = (char *)calloc(sendbuf_size_process, sizeof(char));
    if (rank ==  0) {
      img_recvbuf = (char *)calloc(n_y * n_x, sizeof(char));
    }
  } else { // If the maximum number of iterations is greater than 255, use short int data type for the image
    img_sendbuf = (short int *)calloc(sendbuf_size_process, sizeof(short int));
    if (rank ==  0) {
      img_recvbuf = (short int *)calloc(n_y * n_x , sizeof(short int));
    }
  }

  // Prepare arrays for MPI_Gatherv function to gather the data of the Mandelbrot set
  int *recvcounts = NULL;
  int *displs = NULL;
  if (rank == 0) {
    recvcounts = (int *)calloc(size, sizeof(int));
    displs = (int *)calloc(size, sizeof(int));
  }
  
  // ---------------------- Prepare recvcoounts and displs pointers ----------------------
  // Every process has their own buffer size and displacement
  // We need to gather this information to the root process
  // We use MPI_Gatherv to gather the data from all processes
  int counts_step[size];
  int displs_step[size];

  for (int i = 0; i < size; i++) {
    counts_step[i] = 1;          
    displs_step[i] = i;
  }

  // Time the step gather operations for the recvcounts and displs arrays
  double gather_start_step = MPI_Wtime();
  MPI_Gatherv(&sendbuf_size_process, 1, MPI_INT, recvcounts, counts_step, displs_step, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gatherv(&displ_process, 1, MPI_INT, displs, counts_step, displs_step, MPI_INT, 0, MPI_COMM_WORLD);
  double gather_end_step = MPI_Wtime();
  // -------------------------------------------------------------------------------------
  
  // Record the start time for the parallel computation using OpenMP
  start_time_OMP = omp_get_wtime();

  // Each process generates its portion of the Mandelbrot set
  generate_mandelbrot(n_x, n_y, local_ny_start, local_ny_stop,
                      x_L, y_L, x_R, y_R,
                      I_max, img_sendbuf);

  // Record the end time for the parallel computation using OpenMP
  end_time_OMP = omp_get_wtime();

  // Barrier to make sure that all processes have reached this point
  MPI_Barrier(MPI_COMM_WORLD);

  // Time the gather operation
  double gather_start = MPI_Wtime();

  // Gather data from all processes to the root (rank 0)
  if (I_max < 256){
    MPI_Gatherv(img_sendbuf, // The send buffer
    sendbuf_size_process, // The number of elements to send to each process
    MPI_CHAR, // The data type of the send buffer
    img_recvbuf, // The receive buffer for this process
    recvcounts, // The count of elements each process sends
    displs, // The displacements for each process
    MPI_CHAR, // The data type of the receive buffer
    0, // The rank of the root process
    MPI_COMM_WORLD); // The communicator

  } else {
    MPI_Gatherv(img_sendbuf, // The send buffer
    sendbuf_size_process, // The number of elements to send to each process
    MPI_SHORT, // The data type of the send buffer
    img_recvbuf, // The receive buffer for this process
    recvcounts, // The count of elements each process sends
    displs, // The displacements for each process
    MPI_SHORT, // The data type of the receive buffer
    0, // The rank of the root process
    MPI_COMM_WORLD); // The communicator
  }

  // Record the end time for the gather operation
  double gather_end = MPI_Wtime();

  // Record the end time for the parallel computation using MPI
  end_time_MPI = MPI_Wtime();
  //================================================================================================
  if (size == 1) {
    start_time = start_time_OMP;
    end_time = end_time_OMP;
  } else {
    start_time = start_time_MPI;
    end_time = end_time_MPI;
  }

  // Print the timing information (only on rank 0)
  if (rank == 0) {
    // Print the timing information for the gather operation
    printf("Gather Step Walltime (s): %f\n", gather_end_step - gather_start_step);
    printf("Gather Walltime (s): %f\n", gather_end - gather_start);

    printf("Total Walltime (s): %f\n", end_time - start_time);
    
    // Print the gathered data at the root process
    // Write the data only if the flag is set
    if (WRITE_PGM) {
      write_pgm_image(img_recvbuf, I_max, n_x, n_y, "../plots/mandelbrot_set.pgm");
      printf("The Mandelbrot set has been written\n");
    }

    // Free the allocated memory for the receive buffer
    free(img_recvbuf);
    free(recvcounts);
    free(displs);
  }

  // Free the allocated memory for sendbuf array
  free(img_sendbuf);

  MPI_Finalize();

  return 0;
}