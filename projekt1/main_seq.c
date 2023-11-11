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
    
    
    save_image_png("lena_grayscale.png", lena_grayscale, width, height);

    stbi_image_free(rgb_image);

    //free(lena_grayscale);

    return 0;
}