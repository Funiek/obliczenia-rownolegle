#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define u8 uint8_t
#define u32 uint32_t

typedef struct timespec ts;
typedef struct Pixel {
    u8 r;
    u8 g;
    u8 b;
} Pixel;

void swap(int *a, int *b);
void insertion_sort(u8* arr, int n);
void array2D_free(int** arr, int rows);

#endif // UTILS_H