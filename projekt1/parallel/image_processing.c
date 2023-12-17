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

i8* sobel_operator(i8* pixels, int width, int height) {
    i8* new_pixels = (i8*)malloc(width*height*sizeof(i8));
    i8 gray_color;
    i8 sobel_prep[9];
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

i8* sobel_normalize(i8* pixels, int width, int height) {
    i8* new_pixels = (i8*)malloc(width*height*sizeof(i8));
    i8 max = pixels[0];
    i8 min = pixels[0];

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
            new_pixels[i*width+j] = (i8)(((double)(pixels[(i)*width+j] - min) / (double)(max - min)) * 255.0); // wartość znormalizowana
        }
    }

    return new_pixels;
}

i8* median(i8* pixels, int width, int height, int start, int end) {
    printf("dupa1 %d %d %d\n", start, end, end-start);
    i8* new_pixels = (i8*)malloc((end-start)*sizeof(i8));
    i8 matrix[9];
    printf("dupa2\n");
    // TODO warunek brzegowy
    for(int i = start; i < end; i++) {
        // 0 1 2
        // 3 4 5
        // 6 7 8
        if((i > width) && (i%width != 0) && (i%width != width-1) && ((width*height - width) > i)) {
            matrix[0] = pixels[i-1-width];
            matrix[1] = pixels[i-width];
            matrix[2] = pixels[i+1-width];
            matrix[3] = pixels[i-1];      
            matrix[4] = pixels[i];      
            matrix[5] = pixels[i+1];
            matrix[6] = pixels[i-1+width];
            matrix[7] = pixels[i+width];
            matrix[8] = pixels[i+1+width];

            insertion_sort(matrix, 9);

            new_pixels[i] = matrix[4];
        }
        else {
            new_pixels[i] = pixels[i]; 
        }
        
    }

    printf("dupa3\n");
    return new_pixels;
}

void histogram_values(i8* image, int* histogram, int start, int end) {
    for (int i = start; i < end; i++) {
        histogram[image[i]]++;
    }
}
