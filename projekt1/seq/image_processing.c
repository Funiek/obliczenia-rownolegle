#include "image_processing.h"

void print_image(Pixel* pixels, int width, int height) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("[%hhu %hhu %hhu] ",pixels[i*width+j].r, pixels[i*width+j].g, pixels[i*width+j].b);
        }
        printf("\n");
    }
}

Pixel* convert_to_grayscale(Pixel* pixels, int width, int height) {
    Pixel* new_pixels = (Pixel*)malloc(width*height*sizeof(Pixel));
    u8 gray_color;

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

int multiply_and_add(u8* arr, int* kernel, int N) {
    int sum = 0;
    
    for(int i = 0; i < N; i++) {
        sum += arr[i] * kernel[i];
    }

    return sum;
}

u8* sobel_operator(u8* pixels, int width, int height) {
    u8* new_pixels = (u8*)malloc(width*height*sizeof(u8));
    u8 gray_color;
    u8 sobel_prep[9];
    int x_kernel[9] = {-1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1};
    int y_kernel[9] = {-1, -2, -1,
                       0, 0, 0,
                       1, 2, 1};
    int g_x, g_y, g;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {

            // 0 1 2
            // 3 4 5
            // 6 7 8
            sobel_prep[0] = (i-1 >= 0 && j-1 >= 0) ? pixels[(i-1)*width+j-1] : pixels[(i)*width+j];    
            sobel_prep[1] = (i-1 >= 0) ? pixels[(i-1)*width+j] : pixels[(i)*width+j];   
            sobel_prep[2] = (i-1 >= 0 && j+1 < width) ? pixels[(i-1)*width+j+1] : pixels[(i)*width+j];
            sobel_prep[3] = (j-1 >= 0) ? pixels[(i)*width+j-1] : pixels[(i)*width+j];     
            sobel_prep[4] = pixels[(i)*width+j];      
            sobel_prep[5] = (j+1 < width) ? pixels[(i)*width+j+1] : pixels[(i)*width+j];
            sobel_prep[6] = (i+1 < height && j-1 >= 0) ? pixels[(i+1)*width+j-1] : pixels[(i)*width+j];   
            sobel_prep[7] = (i+1 < height) ? pixels[(i+1)*width+j] : pixels[(i)*width+j];  
            sobel_prep[8] = (i+1 < height && j+1 < width) ? pixels[(i+1)*width+j+1] : pixels[(i)*width+j];

            g_x = multiply_and_add(sobel_prep, x_kernel, 9);
            g_y = multiply_and_add(sobel_prep, y_kernel, 9);
            g = sqrt((g_x * g_x) + (g_y * g_y));

            new_pixels[i*width+j] = g;
        }
    }

    return new_pixels;
}

u8* sobel_normalize(u8* pixels, int width, int height) {
    u8* new_pixels = (u8*)malloc(width*height*sizeof(u8));
    u8 max = pixels[0];
    u8 min = pixels[0];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (pixels[(i)*width+j] > max) {
                max = pixels[(i)*width+j];
            }
            if (pixels[(i)*width+j] < min) {
                min = pixels[(i)*width+j];
            }
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new_pixels[i*width+j] = (u8)(((double)(pixels[(i)*width+j] - min) / (double)(max - min)) * 255.0); // wartość znormalizowana
        }
    }

    return new_pixels;
}

u8* median(u8* pixels, int width, int height) {
    u8* new_pixels = (u8*)malloc(width*height*sizeof(u8));
    u8 matrix[9];

    // TODO warunki brzegowe
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(i>0 && i < height - 1 && j > 0 && j < width - 1) {
                matrix[0] = pixels[(i-1)*width+j-1];    matrix[1] = pixels[(i-1)*width+j];    matrix[2] = pixels[(i-1)*width+j+1];
                matrix[3] = pixels[(i)*width+j-1];      matrix[4] = pixels[(i)*width+j];      matrix[5] = pixels[(i)*width+j+1];
                matrix[6] = pixels[(i+1)*width+j-1];    matrix[7] = pixels[(i+1)*width+j];    matrix[8] = pixels[(i+1)*width+j+1];

                insertion_sort(matrix, 9);

                new_pixels[i*width+j] = matrix[4];
            }
            else {
                new_pixels[i*width+j] = pixels[i*width+j];
            }
            
        }
    }

    return new_pixels;
}

void histogram_values(u8* image, int* histogram, int width, int height) {
    for(int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }


    for(int i = 0; i < height * width; i++) {
        histogram[image[i]]++;
    }
}
