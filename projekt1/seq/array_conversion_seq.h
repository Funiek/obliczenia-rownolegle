#ifndef ARRAY_CONVERSION_SEQ_H
#define ARRAY_CONVERSION_SEQ_H

#include "utils_seq.h"

Pixel* convert_image_to_pixels(i8* rgb_image, int width, int height, int channels);
i8* convert_pixels_to_gray_array(Pixel* pixels, int width, int height);
i8* convert_gray_to_colors_array(i8* image, int width, int height, int channels);

#endif // ARRAY_CONVERSION_SEQ_H