#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/*
The do { ... } while (0) construct is used to create a block that behaves like a single statement. This is a common practice to avoid unexpected issues when using macros in certain contexts.
*/

// Define a macro to resize an array to a new size
#define RESIZE_ARRAY(arr, type, new_size)          \
    do {                                           \
        (arr).size = (new_size);                   \
        (arr).data = (type *)realloc((arr).data, sizeof(type) * (arr).size); \
    } while (0)

// Define a macro to free the memory allocated for an array
#define FREE_ARRAY(arr) free((arr).data)


int main() {
    // Declare an array-like structure template for integers
    ARRAY(int) intArray;

    // Initialize the array with a specified type (int) and initial size (5)
    INIT_ARRAY(intArray, int, 5);

    // Populate the array with values
    for (size_t i = 0; i < intArray.size; ++i) {
        intArray.data[i] = i * 10;
    }

    // Resize the array to a new size (10)
    RESIZE_ARRAY(intArray, int, 10);

    // Populate additional values in the resized array
    for (size_t i = 5; i < intArray.size; ++i) {
        intArray.data[i] = i * 10;
    }

    // Print type of array
    printf("Integer array: ");

    // Print array elements
    for (size_t i = 0; i < intArray.size; ++i) {
        printf("%d ", intArray.data[i]);
    }

    // Free memory
    FREE_ARRAY(intArray);

    // Create a double array
    ARRAY(double) doubleArray;
    INIT_ARRAY(doubleArray, double, 5);

    for (size_t i = 0; i < doubleArray.size; ++i) {
        doubleArray.data[i] = i * 10.0;
    }

    // Resize the array
    RESIZE_ARRAY(doubleArray, double, 10);

    // New line a print type of array
    printf("\n");
    printf("Double array: ");

    for (size_t i = 5; i < doubleArray.size; ++i) {
        doubleArray.data[i] = i * 10.0;
    }

    // Print array elements
    for (size_t i = 0; i < doubleArray.size; ++i) {
        printf("%f ", doubleArray.data[i]);
    }

    // Free memory
    FREE_ARRAY(doubleArray);

    return 0;
}
