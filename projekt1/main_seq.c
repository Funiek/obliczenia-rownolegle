#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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

void print_image(Pixel* pixels, int width, int height) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("[%d %d %d] ",pixels[i*width+j].r, pixels[i*width+j].g, pixels[i*width+j].b);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int width, height, bpp;
    char* image_path = argv[1];

    i8* rgb_image = stbi_load(image_path, &width, &height, &bpp, CHANNELS);
    Pixel* pixels = convert_image_to_pixels(rgb_image, width, height);

    printf("%d %d %d\n\n", width, height, bpp);

    print_image(pixels, 8, 8);

    stbi_image_free(rgb_image);

    return 0;
}