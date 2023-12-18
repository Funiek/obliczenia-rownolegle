#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>

void print_image(Pixel* pixels, int width, int height);
Pixel* convert_to_grayscale(Pixel* pixels, int width, int height);
int multiply_and_add(i8* arr, int* kernel, int N);
i8* sobel_operator(i8* pixels, int width, int height);
i8* sobel_normalize(i8* pixels, int width, int height);
void median(i8* local_pixels, i8* pixels, int width, int height, int start, int end, int rank);
void histogram_values(i8* image, int* histogram, int start, int end);

#endif // IMAGE_PROCESSING_H