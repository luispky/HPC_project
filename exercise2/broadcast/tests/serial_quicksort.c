/*
Implementation of quicksort algorithm in C.

*/

#include <stdio.h>
#include <stdlib.h>

void printArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/*
Recusive quicksort function.

Parameters:
    arr: integer pointer to the array to be sorted
    left: left index of the array
    right: right index of the array

Returns:
    void
*/

void quicksort(int *arr, int left, int right) {
    if (left >= right) {
        return;
    }
    int pivot = arr[(left + right) / 2];
    int i = left;
    int j = right;
    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i <= j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }
    quicksort(arr, left, j);
    quicksort(arr, i, right);
}

int main() {
    int arr[] = {3, 1, 4, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    quicksort(arr, 0, n - 1);
    printArray(arr, n);
    return 0;
}