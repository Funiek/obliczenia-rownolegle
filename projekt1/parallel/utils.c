#include "utils.h"

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertion_sort(u8* arr, int n) {
    int i, key, j;
    for(i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while(j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void array2D_free(int** arr, int rows) {
    for(int i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}