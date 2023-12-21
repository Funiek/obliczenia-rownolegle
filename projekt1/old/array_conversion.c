#include "array_conversion.h"

Pixel* convert_image_to_pixels(i8* rgb_image, int width, int height, int channels) {
    Pixel* pixels = (Pixel*)malloc((width) * (height) * sizeof(Pixel));

    for(int i = 0; i < width*height*channels; i+=channels) {
        pixels[i/channels].r = rgb_image[i];
        pixels[i/channels].g = rgb_image[i+1];
        pixels[i/channels].b = rgb_image[i+2];
    }

    return pixels;
}

i8* convert_pixels_to_gray_array(Pixel* pixels, int width, int height) {
    i8* image = (i8*)malloc(width*height*sizeof(i8));

    for(int i = 0; i < width*height; i++) {
        image[i] = pixels[i].r;
    }

    return image;
}

i8* convert_gray_to_colors_array(i8* image, int width, int height, int channels) {
    i8* rgb_image = (i8*)malloc(width*height*channels*sizeof(i8));

    for(int i = 0; i < width*height*channels; i+=channels) {
        rgb_image[i] = image[i/channels];
        rgb_image[i+1] = image[i/channels];
        rgb_image[i+2] = image[i/channels];
    }

    return rgb_image;
}