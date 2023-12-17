#include <stdio.h>
#include <stdlib.h>
#include "parallel/utils.h"
#include "parallel/array_conversion.h"
#include "parallel/image_processing.h"
#include "mpi.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define CHANNELS 3

int save_image_png(char const* file_name, i8* rgb_image, int width, int height) {
    return stbi_write_png(file_name, width, height, CHANNELS, rgb_image, width * CHANNELS);
}

int main(int argc, char **argv)
{
    // parametry obrazka
    int width, height, bpp;
    i8* rgb_image;
    Pixel* pixels;

    // skala szarości
    Pixel* grayscale_pixels;
    i8* grayscale;
    i8* grayscale_in_RGB;

    // histogram
    int histogram[256] = {0};

    // sobel
    i8* sobel_operator_result;
    i8* sobel_normalize_result;
    i8* sobel;

    // mediana
    i8* image_median_result;
    i8* image_median;
    
    // nazwa zdjecia
    char* image_path = argv[1];
    char image_name[100];
    strcpy(image_name, image_path);

    image_name[strlen(image_path)-4] = '\0';

    // stringi na nazwy filtrów do zapisu do plików
    char image_path_gray[strlen(image_name)+15];
    char image_path_sobel[strlen(image_name)+13];
    char image_path_median[strlen(image_name)+13];

    // czasy
    ts sobel_t1, sobel_t2, median_t1, median_t2, histogram_t1, histogram_t2;

    // dodanie prefixa z nazwą folderu
    strcpy(image_path_gray, "img/");
    strcpy(image_path_sobel, "img/");
    strcpy(image_path_median, "img/");

    // dodanie suffixów z nazwami wykonanych czynności
    strcat(image_path_gray, image_name);
    strcat(image_path_gray,"_grayscale.png");

    strcat(image_path_sobel, image_name);
    strcat(image_path_sobel,"_sobel.png");

    strcat(image_path_median, image_name);
    strcat(image_path_median,"_median.png");

    // konwersja zdjęcia do tablicy intów
    rgb_image = stbi_load(image_path, &width, &height, &bpp, CHANNELS);

    // konwersja na structa Pixel reprezentującego wartości R,G,B w tablicy per pixel
    pixels = convert_image_to_pixels(rgb_image, width, height, CHANNELS);
    printf("%d %d %d\n\n", width, height, bpp);

    // konwersja do skali szarości
    grayscale_pixels = convert_to_grayscale(pixels, width, height);
    grayscale = convert_pixels_to_gray_array(grayscale_pixels, width, height);
    grayscale_in_RGB = convert_gray_to_colors_array(grayscale, width, height, CHANNELS);
    
    // zapis zdjęcia w skali szarości do pliku
    save_image_png(image_path_gray, grayscale_in_RGB, width, height);
    
    // filtr z wykorzystaniem operatora sobela
    clock_gettime(CLOCK_MONOTONIC_RAW, &sobel_t1);
    sobel_operator_result = sobel_operator(grayscale, width, height);
    sobel_normalize_result = sobel_normalize(sobel_operator_result, width, height);
    clock_gettime(CLOCK_MONOTONIC_RAW, &sobel_t2);

    
    
    // zapis zdjęcia po filtrze z operatorem sobela do pliku
    sobel = convert_gray_to_colors_array(sobel_normalize_result, width, height, CHANNELS);
    save_image_png(image_path_sobel, sobel, width, height);


    // obliczenie histogramu
    clock_gettime(CLOCK_MONOTONIC_RAW, &histogram_t1);
    histogram_values(grayscale, histogram, width, height);
    clock_gettime(CLOCK_MONOTONIC_RAW, &histogram_t2);

    // wypisanie histogramu
    // for(int i = 0; i < 256; i++) {
    //     printf("%d. Natężenie: %d\n", i, histogram[i]);
    // }

    // zastosowanie filtru medianowego (nowa tablica wynikowa)
    clock_gettime(CLOCK_MONOTONIC_RAW, &median_t1);
    image_median_result = median(grayscale, width, height);
    clock_gettime(CLOCK_MONOTONIC_RAW, &median_t2);
    
    // zapis zdjęcia z zastosowanym filtrem medianowym
    image_median = convert_gray_to_colors_array(image_median_result, width, height, CHANNELS);
    save_image_png(image_path_median, image_median, width, height);

    printf("Czasy:\nHistogram: %ld\nSobel: %ld\nMedian: %ld\n", diff_ts(histogram_t1, histogram_t2).tv_nsec, diff_ts(sobel_t1, sobel_t2).tv_nsec, diff_ts(median_t1, median_t2).tv_nsec);

    // zwalnianie pamięci
    stbi_image_free(rgb_image);
    free(sobel);
    free(grayscale);
    free(image_median);
    free(pixels);
    free(grayscale_pixels);
    free(sobel_operator_result);
    free(sobel_normalize_result);
    free(image_median_result);

    return 0;
}