// #include <stdio.h>
// #include <stdlib.h>
// #include <omp.h>

// #define N_DEFAULT 100
// #define DEFAULT_NUM_THREADS 4
// #define MAX_RANDOM_VALUE 10000



// // Write a function that allows me copy the data from one array to another using pointers
// void copy_array(int* array, int* array_copy, int size){
//   for(int i = 0; i < size; i++){
//     *(array_copy + i) = *(array + i);
//   }
// }

// void b_cast(int* array, int size, int root){
//   int rank = omp_get_thread_num();
//   int nthreads = omp_get_num_threads();
//   if(rank != root){
//     int *arrCopy = malloc(size * sizeof(int));
//     memcpy(arrCopy, array, size * sizeof(int));
//   }
// }

// int* generateRandomArray(int size) {
//     int *array = (int*)malloc(sizeof(int) * size);

//     if (array == NULL) {
//         // Handle memory allocation failure
//         printf("Memory allocation failed\n");
//         exit(EXIT_FAILURE);
//     }

//     // Seed the random number generator with the current time
//     srand((unsigned int)time(NULL));

//     // Fill the array with random numbers within the specified limit
//     for (int i = 0; i < size; ++i) {
//         array[i] = rand() % (MAX_RANDOM_VALUE + 1);
//     }

//     return array;
// }


#include <omp.h>
#include <stdio.h>

int sharedData = 0;

int main() {
    // OpenMP parallel region
    #pragma omp parallel
    {
        // Get thread ID
        int tid = omp_get_thread_num();

        // Only one thread updates the shared data to 42
        if (tid == 0) {
            #pragma omp atomic write
            sharedData = 42;
        }

        // All threads print the current value of sharedData
        #pragma omp barrier
        printf("Thread %d: Shared Data = %d\n", tid, sharedData);
    }

    return 0;
}
