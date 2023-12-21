#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>

void print_image(Pixel* pixels, int width, int height);
Pixel* convert_to_grayscale(Pixel* pixels, int width, int height);
int multiply_and_add(u8* arr, int* kernel, int N);
u8* sobel_operator(const u8* pixels, int width, int height);
u8* sobel_normalize(const u8* pixels, int width, int height);
u8* median(const u8* pixels, int width, int height);
void histogram_values(const u8* image, int* histogram, int width, int height);

#endif // IMAGE_PROCESSING_H