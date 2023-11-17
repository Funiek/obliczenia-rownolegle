#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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

void insertion_sort(i8* arr, int n) {
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

int multiply_and_add(i8* arr, int* kernel, int N) {
    int sum = 0;
    
    for(int i = 0; i < N; i++) {
        sum += arr[i] * kernel[i];
    }

    return sum;
}

Pixel* sobel_operator(Pixel* pixels, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(width*height*sizeof(Pixel));
    i8 gray_color;
    i8 sobel_prep[9];
    int x_kernel[9] = {-1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1};
    int y_kernel[9] = {-1, -2, -1,
                       0, 0, 0,
                       1, 2, 1};
    int g_x, g_y, g;

    for(int i = 1; i < height - 1; i++) {
        for(int j = 1; j < width - 1; j++) {
            //gray_color = pixels[i*width+j].r;

            sobel_prep[0] = pixels[(i-1)*width+j-1].r;    sobel_prep[1] = pixels[(i-1)*width+j].r;    sobel_prep[2] = pixels[(i-1)*width+j+1].r;
            sobel_prep[3] = pixels[(i)*width+j-1].r;      sobel_prep[4] = pixels[(i)*width+j].r;      sobel_prep[5] = pixels[(i)*width+j+1].r;
            sobel_prep[6] = pixels[(i+1)*width+j-1].r;    sobel_prep[7] = pixels[(i+1)*width+j].r;    sobel_prep[8] = pixels[(i+1)*width+j+1].r;

            g_x = multiply_and_add(sobel_prep, x_kernel, 9);
            g_y = multiply_and_add(sobel_prep, y_kernel, 9);
            g = sqrt((g_x * g_x) + (g_y * g_y));


            new_pixels[i*width+j].r = g;
            new_pixels[i*width+j].g = g;
            new_pixels[i*width+j].b = g;
        }
    }

    return new_pixels;
}

Pixel* sobel_normalize(Pixel* pixels, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(width*height*sizeof(Pixel));
    i8 max = pixels[0].r;
    i8 min = pixels[0].r;
    i8 norm_value;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (pixels[(i)*width+j].r > max) {
                max = pixels[(i)*width+j].r;
            }
            if (pixels[(i)*width+j].r < min) {
                min = pixels[(i)*width+j].r;
            }
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            norm_value = (i8)(((double)(pixels[(i)*width+j].r - min) / (double)(max - min)) * 255.0);

            new_pixels[i*width+j].r = norm_value;
            new_pixels[i*width+j].g = norm_value;
            new_pixels[i*width+j].b = norm_value;
        }
    }

    return new_pixels;
}

Pixel* median(Pixel* pixels, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(width*height*sizeof(Pixel));
    i8 matrix_r[9];
    i8 matrix_g[9];
    i8 matrix_b[9];

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
    char image_name[100];
    strcpy(image_name, image_path);
    image_name[strlen(image_path)-4] = '\0';

    char image_path_gray[strlen(image_name)+11];
    char image_path_sobel[strlen(image_name)+7];
    char image_path_median[strlen(image_name)+8];
    printf("%s %s %ld\n", image_path, image_name, strlen(image_name));

    i8* rgb_image = stbi_load(image_path, &width, &height, &bpp, CHANNELS);
    Pixel* pixels = convert_image_to_pixels(rgb_image, width, height);

    printf("%d %d %d\n\n", width, height, bpp);

    Pixel* grayscale_pixels = convert_to_grayscale(pixels, width, height);
    i8* grayscale = convert_pixels_to_image(grayscale_pixels, width, height);

    printf("%s %s %ld\n", image_path, image_name, strlen(image_name));
    strcpy(image_path_gray, image_name);
    strcat(image_path_gray,"_grayscale.png");
    save_image_png(image_path_gray, grayscale, width, height);
    
    Pixel* sobel_operator_pixels = sobel_operator(grayscale_pixels, width, height);
    Pixel* sobel_pixels = sobel_normalize(sobel_operator_pixels, width, height);
    i8* sobel = convert_pixels_to_image(sobel_pixels, width, height);

    printf("%s %s %ld\n", image_path, image_name, strlen(image_name));
    strcpy(image_path_sobel, image_name);
    strcat(image_path_sobel,"_sobel.png");
    save_image_png(image_path_sobel, sobel, width, height);


    int** histogram = histogram_values(pixels, width, height);

    // for(int i = 0; i < 256; i++) {
    //     printf("%d. r: %d g: %d b: %d\n", i, histogram[0][i], histogram[1][i], histogram[2][i]);
    // }


    Pixel* image_median_pixels = median(pixels, width, height);
    i8* image_median = convert_pixels_to_image(image_median_pixels, width, height);

    printf("%s %s %ld\n", image_path, image_name, strlen(image_name));
    strcpy(image_path_median, image_name);
    strcat(image_path_median,"_median.png");
    save_image_png(image_path_median, image_median, width, height);

    stbi_image_free(rgb_image);
    stbi_image_free(sobel);
    stbi_image_free(grayscale);
    stbi_image_free(image_median);
    free(pixels);
    free(grayscale_pixels);
    free(sobel_operator_pixels);
    free(sobel_pixels);
    free(image_median_pixels);
    array2D_free(histogram, CHANNELS);

    return 0;
}