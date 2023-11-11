#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define i8 uint8_t
#define CHANNELS 3

typedef struct Pixel {
    i8 r;
    i8 g;
    i8 b;
} Pixel;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertion_sort(int* arr, int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

Pixel* convert_image_to_pixels(i8* rgb_image, int width, int height) {
    Pixel* pixels = (Pixel*)malloc(width*height*sizeof(Pixel));

    for(int i = 0; i < width*height*CHANNELS; i+=CHANNELS) {
        pixels[i/CHANNELS].r = rgb_image[i];
        pixels[i/CHANNELS].g = rgb_image[i+1];
        pixels[i/CHANNELS].b = rgb_image[i+2];
    }

    return pixels;
}

i8* convert_pixels_to_image(Pixel* pixels, int width, int height) {
    i8* rgb_image = (i8*)malloc(width*height*CHANNELS*sizeof(i8));

    for(int i = 0; i < width*height*CHANNELS; i+=CHANNELS) {
        rgb_image[i] = pixels[i/CHANNELS].r;
        rgb_image[i+1] = pixels[i/CHANNELS].g;
        rgb_image[i+2] = pixels[i/CHANNELS].b;
    }

    return rgb_image;
}

int save_image_png(char const* file_name, i8* rgb_image, int width, int height) {
    return stbi_write_png(file_name, width, height, CHANNELS, rgb_image, width * CHANNELS);
}

void print_image(Pixel* pixels, int width, int height) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("[%d %d %d] ",pixels[i*width+j].r, pixels[i*width+j].g, pixels[i*width+j].b);
        }
        printf("\n");
    }
}

Pixel* convert_to_grayscale(Pixel* pixels, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(width*height*sizeof(Pixel));
    i8 gray_color;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            gray_color = pixels[i*width+j].r*0.299 + pixels[i*width+j].g*0.587 + pixels[i*width+j].b*0.114;

            new_pixels[i*width+j].r = gray_color;
            new_pixels[i*width+j].g = gray_color;
            new_pixels[i*width+j].b = gray_color;
        }
    }

    return new_pixels;
}

Pixel* median(Pixel* pixels, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(width*height*sizeof(Pixel));
    int matrix_r[9];
    int matrix_g[9];
    int matrix_b[9];

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(i>0 && i < height - 1 && j > 0 && j < width - 1) {
                matrix_r[0] = pixels[(i-1)*width+j-1].r;    matrix_r[1] = pixels[(i-1)*width+j].r;    matrix_r[2] = pixels[(i-1)*width+j+1].r;
                matrix_r[3] = pixels[(i)*width+j-1].r;      matrix_r[4] = pixels[(i)*width+j].r;      matrix_r[5] = pixels[(i)*width+j+1].r;
                matrix_r[6] = pixels[(i+1)*width+j-1].r;    matrix_r[7] = pixels[(i+1)*width+j].r;    matrix_r[8] = pixels[(i+1)*width+j+1].r;

                matrix_g[0] = pixels[(i-1)*width+j-1].g;    matrix_g[1] = pixels[(i-1)*width+j].g;    matrix_g[2] = pixels[(i-1)*width+j+1].g;
                matrix_g[3] = pixels[(i)*width+j-1].g;      matrix_g[4] = pixels[(i)*width+j].g;      matrix_g[5] = pixels[(i)*width+j+1].g;
                matrix_g[6] = pixels[(i+1)*width+j-1].g;    matrix_g[7] = pixels[(i+1)*width+j].g;    matrix_g[8] = pixels[(i+1)*width+j+1].g;

                matrix_b[0] = pixels[(i-1)*width+j-1].b;    matrix_b[1] = pixels[(i-1)*width+j].b;    matrix_b[2] = pixels[(i-1)*width+j+1].b;
                matrix_b[3] = pixels[(i)*width+j-1].b;      matrix_b[4] = pixels[(i)*width+j].b;      matrix_b[5] = pixels[(i)*width+j+1].b;
                matrix_b[6] = pixels[(i+1)*width+j-1].b;    matrix_b[7] = pixels[(i+1)*width+j].b;    matrix_b[8] = pixels[(i+1)*width+j+1].b;

                insertion_sort(matrix_r, 9);
                insertion_sort(matrix_g, 9);
                insertion_sort(matrix_b, 9);

                new_pixels[i*width+j].r = matrix_r[4];
                new_pixels[i*width+j].g = matrix_g[4];
                new_pixels[i*width+j].b = matrix_b[4];
            }
            else {
                new_pixels[i*width+j].r = pixels[i*width+j].r;
                new_pixels[i*width+j].g = pixels[i*width+j].g;
                new_pixels[i*width+j].b = pixels[i*width+j].b;
            }
            
        }
    }

    return new_pixels;
}

void array2D_free(int** arr, int rows) {
    for(int i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}

int** histogram_values(Pixel* pixels, int width, int height) {
    int** histogram = (int**)malloc(CHANNELS * sizeof(int*));
    for (int i = 0; i < CHANNELS; i++) {
        histogram[i] = (int*)malloc(256 * sizeof(int));
    }


    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            histogram[0][pixels[i*width+j].r]++;
            histogram[1][pixels[i*width+j].g]++;
            histogram[2][pixels[i*width+j].b]++;
        }
    }

    return histogram;
}

int main(int argc, char **argv)
{
    int width, height, bpp;
    char* image_path = argv[1];

    i8* rgb_image = stbi_load(image_path, &width, &height, &bpp, CHANNELS);
    Pixel* pixels = convert_image_to_pixels(rgb_image, width, height);

    printf("%d %d %d\n\n", width, height, bpp);

    Pixel* lena_grayscale_pixels = convert_to_grayscale(pixels, width, height);
    i8* lena_grayscale = convert_pixels_to_image(lena_grayscale_pixels, width, height);
    // print_image(pixels, 8, 8);
    
    
    // save_image_png("lena_grayscale.png", lena_grayscale, width, height);


    int** histogram = histogram_values(pixels, width, height);

    // for(int i = 0; i < 256; i++) {
    //     printf("%d. r: %d g: %d b: %d\n", i, histogram[0][i], histogram[1][i], histogram[2][i]);
    // }


    Pixel* mak_median_pixels = median(pixels, width, height);
    i8* mak_median = convert_pixels_to_image(mak_median_pixels, width, height);
    save_image_png("mak_median.png", mak_median, width, height);

    stbi_image_free(rgb_image);
    stbi_image_free(lena_grayscale);
    free(pixels);
    free(lena_grayscale_pixels);
    array2D_free(histogram, CHANNELS);

    return 0;
}