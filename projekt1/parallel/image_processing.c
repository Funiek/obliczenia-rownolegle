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

u8* sobel_operator(const u8* pixels, int width, int height, int start, int end, int rank) {
    u8* new_pixels = (u8*)malloc((end-start)*sizeof(u8));
    
    u8 sobel_prep[9];
    bool top_touched, bottom_touched, left_touched, right_touched;

    int x_kernel[9] = {-1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1};
    int y_kernel[9] = {-1, -2, -1,
                       0, 0, 0,
                       1, 2, 1};
    int g_x, g_y, g;

    for(int i = start; i < end; i++) {
        top_touched = (i-width < 0) ? true : false;
        bottom_touched = (i+width >= width*height) ? true : false;
        left_touched = ((i-1)%width == width-1) ? true : false;
        right_touched = ((i+1)%width == 0) ? true : false;
        
        // 0 1 2
        // 3 4 5
        // 6 7 8
        sobel_prep[0] = (!left_touched && !top_touched) ? pixels[i-1-width] : pixels[i];
        sobel_prep[1] = (!top_touched) ? pixels[i-width] : pixels[i];
        sobel_prep[2] = (!top_touched && !right_touched) ? pixels[i+1-width] : pixels[i];
        sobel_prep[3] = (!left_touched) ? pixels[i-1] : pixels[i];
        sobel_prep[4] = pixels[i];
        sobel_prep[5] = (!right_touched) ? pixels[i+1] : pixels[i];
        sobel_prep[6] = (!left_touched && !bottom_touched) ? pixels[i-1+width] : pixels[i];
        sobel_prep[7] = (!bottom_touched) ?  pixels[i+width] : pixels[i];
        sobel_prep[8] = (!bottom_touched && !right_touched) ? pixels[i+1+width] : pixels[i];

        g_x = multiply_and_add(sobel_prep, x_kernel, 9);
        g_y = multiply_and_add(sobel_prep, y_kernel, 9);
        g = sqrt((g_x * g_x) + (g_y * g_y));

        new_pixels[i - start] = g;
    }

    return new_pixels;
}

u8* sobel_normalize(const u8* pixels, int start, int end, int rank) {
    u8* new_pixels = (u8*)malloc((end-start)*sizeof(u8));
    u8 max = pixels[0];
    u8 min = pixels[0];

    for(int i = 0; i < end - start; i++) {
        if (pixels[i] > max) {
            max = pixels[i];
        }
        if (pixels[i] < min) {
            min = pixels[i];
        }
    }

    for(int i = 0; i < end - start; i++) {
        new_pixels[i] = (u8)(((double)(pixels[i] - min) / (double)(max - min)) * 255.0); // wartość znormalizowana
    }

    return new_pixels;
}

u8* median(const u8* pixels, int width, int height, int start, int end, int rank) {
    u8* local_pixels = (u8*)malloc((end-start)*sizeof(u8));

    u8 matrix[9];
    bool top_touched, bottom_touched, left_touched, right_touched;

    for(int i = start; i < end; i++) {
        // 0 1 2
        // 3 4 5
        // 6 7 8
        top_touched = (i-width < 0) ? true : false;
        bottom_touched = (i+width >= width*height) ? true : false;
        left_touched = ((i-1)%width == width-1) ? true : false;
        right_touched = ((i+1)%width == 0) ? true : false;
        
        matrix[0] = (!left_touched && !top_touched) ? pixels[i-1-width] : pixels[i];
        matrix[1] = (!top_touched) ? pixels[i-width] : pixels[i];
        matrix[2] = (!top_touched && !right_touched) ? pixels[i+1-width] : pixels[i];
        matrix[3] = (!left_touched) ? pixels[i-1] : pixels[i];
        matrix[4] = pixels[i];
        matrix[5] = (!right_touched) ? pixels[i+1] : pixels[i];
        matrix[6] = (!left_touched && !bottom_touched) ? pixels[i-1+width] : pixels[i];
        matrix[7] = (!bottom_touched) ?  pixels[i+width] : pixels[i];
        matrix[8] = (!bottom_touched && !right_touched) ? pixels[i+1+width] : pixels[i];

        insertion_sort(matrix, 9);

        local_pixels[i - start] = matrix[4];   
    }

    return local_pixels;
}

void histogram_values(const u8* image, u32* histogram, int start, int end) {
    for (int i = start; i < end; i++) {
        histogram[image[i]]++;
    }
}
