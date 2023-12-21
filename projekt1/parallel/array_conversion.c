#include "array_conversion.h"

Pixel* convert_image_to_pixels(u8* rgb_image, int width, int height, int channels) {
    Pixel* pixels = (Pixel*)malloc((width) * (height) * sizeof(Pixel));

    for(int i = 0; i < width*height*channels; i+=channels) {
        pixels[i/channels].r = rgb_image[i];
        pixels[i/channels].g = rgb_image[i+1];
        pixels[i/channels].b = rgb_image[i+2];
    }

    return pixels;
}

u8* convert_pixels_to_gray_array(Pixel* pixels, int width, int height) {
    u8* image = (u8*)malloc(width*height*sizeof(u8));

    for(int i = 0; i < width*height; i++) {
        image[i] = pixels[i].r;
    }

    return image;
}

u8* convert_gray_to_colors_array(u8* image, int width, int height, int channels) {
    u8* rgb_image = (u8*)malloc(width*height*channels*sizeof(u8));

    for(int i = 0; i < width*height*channels; i+=channels) {
        rgb_image[i] = image[i/channels];
        rgb_image[i+1] = image[i/channels];
        rgb_image[i+2] = image[i/channels];
    }

    return rgb_image;
}