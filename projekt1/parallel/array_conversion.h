#ifndef ARRAY_CONVERSION_H
#define ARRAY_CONVERSION_H

#include "utils.h"

Pixel* convert_image_to_pixels(u8* rgb_image, int width, int height, int channels);
u8* convert_pixels_to_gray_array(Pixel* pixels, int width, int height);
u8* convert_gray_to_colors_array(u8* image, int width, int height, int channels);

#endif // ARRAY_CONVERSION_H
