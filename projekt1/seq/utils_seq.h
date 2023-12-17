#ifndef UTILS_SEQ_H
#define UTILS_SEQ_H

#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define i8 uint8_t

typedef struct timespec ts;
typedef struct Pixel {
    i8 r;
    i8 g;
    i8 b;
} Pixel;

void swap(int *a, int *b);
void insertion_sort(i8* arr, int n);
ts diff_ts(ts start, ts end);
void array2D_free(int** arr, int rows);

#endif // UTILS_SEQ_H