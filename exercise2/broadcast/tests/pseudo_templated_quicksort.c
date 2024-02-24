#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX_RANDOM_VALUE 1000

// Define a macro to create an array-like structure template
#define ARRAY(T)        \
    struct {             \
        size_t size;     \
        T *data;         \
    }

// Define a macro to initialize an array with a specified type and initial size
#define INIT_ARRAY(arr, type, initial_size)        \
    do {                                           \
        (arr).size = (initial_size);               \
        (arr).data = (type *)malloc(sizeof(type) * (arr).size); \
    } while (0)

// Define a macro to resize an array to a new size
#define RESIZE_ARRAY(arr, type, new_size)          \
    do {                                           \
        (arr).size = (new_size);                   \
        (arr).data = (type *)realloc((arr).data, sizeof(type) * (arr).size); \
    } while (0)

// Define a macro to free the memory allocated for an array
#define FREE_ARRAY(arr) free((arr).data)

void swap(int *x, int *y);
void serial_quicksort(ARRAY(int) array);
void serial_quicksort_recursion(ARRAY(int) array, int low, int high);
void parallel_quicksort(ARRAY(int) array);
void parallel_quicksort_recursion(ARRAY(int) array, int low, int high);
int partition(ARRAY(int) array, int low, int high);
void printArray(ARRAY(int) arr);
ARRAY(int) generateRandomArray(int size);

int main() {
    clock_t start_cpu, end_cpu;
    time_t start_wall, end_wall;

    int length = 10000000; // 10^7

    // Serial quicksort
    {
        // Generate a random array
        ARRAY(int) a;
        INIT_ARRAY(a, int, length);

        // time the serial quicksort
        start_cpu = clock();
        time(&start_wall);

        // use quicksort to sort the array
        serial_quicksort(a);

        // Record end times
        end_cpu = clock();
        time(&end_wall);

        // Print out the time it took to sort the array
        printf("Serial Quicksort CPU time (s): %f\n", (double)(end_cpu - start_cpu) / CLOCKS_PER_SEC);
        printf("Serial Quicksort Wall time (s): %f\n", (double)difftime(end_wall, start_wall));
        // print out the array to ensure it has been sorted
        // printArray(a);

        FREE_ARRAY(a);
    }

    // Parallel Quicksort
    {
        // Generate a random array
        ARRAY(int) a;
        INIT_ARRAY(a, int, length);

        // time the parallel quicksort
        start_cpu = clock();
        time(&start_wall);

        // use quicksort to sort the array
        parallel_quicksort(a);

        // Record end times
        end_cpu = clock();
        time(&end_wall);

        // Print out the time it took to sort the array
        printf("Parallel Quicksort CPU time (s): %f\n", (double)(end_cpu - start_cpu) / CLOCKS_PER_SEC);
        printf("Parallel Quicksort Wall time (s): %f\n", (double)difftime(end_wall, start_wall));
        // print out the array to ensure it has been sorted
        // printArray(a);

        // Free the memory
        FREE_ARRAY(a);
    }

    return 0;
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void serial_quicksort(ARRAY(int) array) {
    srand(time(NULL));

    serial_quicksort_recursion(array, 0, array.size - 1);
}

void serial_quicksort_recursion(ARRAY(int) array, int low, int high) {
    if (low < high) {
        int pivot_index = partition(array, low, high);

        serial_quicksort_recursion(array, low, pivot_index - 1);

        serial_quicksort_recursion(array, pivot_index + 1, high);
    }
}

int partition(ARRAY(int) array, int low, int high) {
    int pivot_index = low + (rand() % (high - low));

    if (pivot_index != high)
        // time complexity of swap is O(1) since accessing array elements is O(1)
        swap(&array.data[pivot_index], &array.data[high]);

    int pivot_value = array.data[high];

    int i = low;
    for (int j = low; j < high; j++) {
        if (array.data[j] <= pivot_value) {
            swap(&array.data[i], &array.data[j]);
            i++;
        }
    }

    swap(&array.data[i], &array.data[high]);

    return i;
}

void printArray(ARRAY(int) arr) {
    for (int i = 0; i < arr.size; i++) {
        printf("%d ", arr.data[i]);
    }
    printf("\n");
}

void parallel_quicksort(ARRAY(int) array) {
    srand(time(NULL));

#pragma omp parallel
#pragma omp single
    parallel_quicksort_recursion(array, 0, array.size - 1);
}

void parallel_quicksort_recursion(ARRAY(int) array, int low, int high) {
    if (low < high) {
        int pivot_index = partition(array, low, high);
#pragma omp task
        parallel_quicksort_recursion(array, low, pivot_index - 1);
#pragma omp task
        parallel_quicksort_recursion(array, pivot_index + 1, high);
    }
}

ARRAY(int) generateRandomArray(int size) {
    ARRAY(int) array;
    INIT_ARRAY(array, int, size);

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Fill the array with random numbers within the specified limit
    for (int i = 0; i < size; ++i) {
        array.data[i] = rand() % (MAX_RANDOM_VALUE + 1);
    }

    return array;
}
