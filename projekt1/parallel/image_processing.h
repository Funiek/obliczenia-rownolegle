#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>
#include <stdbool.h>

void print_image(Pixel* pixels, int width, int height);
Pixel* convert_to_grayscale(Pixel* pixels, int width, int height);
int multiply_and_add(u8* arr, int* kernel, int N);
u8* sobel_operator(u8* pixels, int width, int height, int start, int end, int rank);
u8* sobel_normalize(u8* pixels, int start, int end, int rank);
u8* median(u8* pixels, int width, int height, int start, int end, int rank);
void median2(u8* local_pixels, u8* pixels, int interval);
void histogram_values(const u8* image, int* histogram, int start, int end);

#endif // IMAGE_PROCESSING_H