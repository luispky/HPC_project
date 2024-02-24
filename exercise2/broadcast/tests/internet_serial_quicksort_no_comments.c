#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <string.h>

// compile with:
// ! gcc internet_serial_quicksort_no_comments.c -fopenmp 

#define MAX_RANDOM_VALUE 10000
#define N_DEFAULT 1000000 // 10^6
#define DEFAULT_NUM_THREADS 12   // Default number of threads

void swap(int *x, int *y);
void serial_quicksort(int* array, int length);
void serial_quicksort_recursion(int* array, int low, int high);
void parallel_quicksort(int* array, int length);
void parallel_quicksort_recursion(int* array, int low, int high);
int partition(int* array, int low, int high);
void printArray(int *arr, int n);
int* generateRandomArray(int size);


int main(int argc, char *argv[]){
  
  int size = (argc>1) ? atoi(*(argv+1)) : N_DEFAULT;
  int num_threads = (argc>2) ? atoi(*(argv+2)) : DEFAULT_NUM_THREADS;
  omp_set_num_threads(num_threads); 

  // Generate a random array
  int *a = generateRandomArray(size);
  int *aCopy = malloc(size * sizeof(int));
  memcpy(aCopy, a, size * sizeof(int));
  
  // Serial quicksort
  {
    // time the serial quicksort
    clock_t start_cpu_serial = clock();
    double start_wall_serial = omp_get_wtime();

    // use quicksort to sort the array
    //! it automatically takes the address of the first element of the array
    serial_quicksort(a, size);
    // print out the array to ensure it has been sorted
    // Print serial result
    // printf("Serial result: ");
    // printArray(a, size);

    clock_t end_cpu_serial = clock();
    double end_wall_serial = omp_get_wtime();
    printf("Serial CPU time (s): %f\n", (double)(end_cpu_serial - start_cpu_serial) / CLOCKS_PER_SEC);
    printf("Serial Wall time (s): %f\n", end_wall_serial - start_wall_serial);
    printf("\n");

  }

  // Parallel Quicksort
  {
    clock_t start_cpu_parallel = clock();
    double start_wall_parallel = omp_get_wtime();
  
    // use quicksort to sort the array
    parallel_quicksort(aCopy, size);

    // Print parallel result
    // printf("Parallel result: ");
    // printArray(aCopy, size);

    clock_t end_cpu_parallel = clock();
    double end_wall_parallel = omp_get_wtime();

    printf("Parallel CPU time (s): %f\n", (double)(end_cpu_parallel - start_cpu_parallel) / CLOCKS_PER_SEC);
    printf("Parallel Wall time (s): %f\n", end_wall_parallel - start_wall_parallel);
    printf("\n");

  }


  free(a);
  free(aCopy);
  return 0;
}

void swap(int *x, int *y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}

/*
!Arrays Passed by Reference:
*In C, when you pass an array to a function, you are actually passing a pointer to the first element of the array. This is why changes made to the array inside the function are reflected outside the function.
*/

/*
!Pointer Syntax:

When you see function parameters declared with array syntax, like 
*void myFunction(int arr[]), it's equivalent to void myFunction(int *arr). 
This syntax emphasizes that the parameter is a pointer.
*/

void serial_quicksort(int* array, int length)
{
/*
*When you declare a function parameter as an array, the size information is not passed along with it. For example, if you have a function void myFunction(int arr[]), the function receives a pointer to the first element, and it doesn't know the size of the array. 
!This is why in your quicksort function, you need to pass the length separately.
*/

  srand(time(NULL));
  
  serial_quicksort_recursion(array, 0, length - 1);
}

/*
*The quicksort and quicksort_recursion functions take an array and its length as parameters. The array is effectively passed by reference, allowing the quicksort algorithm to modify the array in place.
*/

void serial_quicksort_recursion(int* array, int low, int high)
{
  if (low < high)
  {
    int pivot_index = partition(array, low, high);

    serial_quicksort_recursion(array, low, pivot_index - 1);

    serial_quicksort_recursion(array, pivot_index + 1, high);
  }
}

int partition(int* array, int low, int high)
{
  int pivot_index = low + (rand() % (high - low));
  
  if (pivot_index != high) 
    // time complexity of swap is O(1) since accessing array elements is O(1)
    swap(&array[pivot_index], &array[high]);
  
  int pivot_value = array[high];
  
  int i = low; 
  for (int j = low; j < high; j++)
  {
    if (array[j] <= pivot_value)
    {
      swap(&array[i], &array[j]);
      i++;
    }
  }
  
  swap(&array[i], &array[high]);
  
  return i;
}

void printArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void parallel_quicksort(int* array, int length)
{
  srand(time(NULL));

#pragma omp parallel
#pragma omp single 
  parallel_quicksort_recursion(array, 0, length - 1);
}

void parallel_quicksort_recursion(int* array, int low, int high)
{
  if (low < high)
  {
    int pivot_index = partition(array, low, high);
    #pragma omp task
      parallel_quicksort_recursion(array, low, pivot_index - 1);
    #pragma omp task
      parallel_quicksort_recursion(array, pivot_index + 1, high);
  }
}

// Function to generate an array of random numbers
int* generateRandomArray(int size) {
    int *array = (int*)malloc(sizeof(int) * size);

    if (array == NULL) {
        // Handle memory allocation failure
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Fill the array with random numbers within the specified limit
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % (MAX_RANDOM_VALUE + 1);
    }

    return array;
}