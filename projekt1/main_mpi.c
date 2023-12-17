#include <stdio.h>
#include <stdlib.h>
#include "seq/utils.h"
#include "seq/array_conversion.h"
#include "seq/image_processing.h"
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
    int local_start, local_end;
    i8* rgb_image;
    Pixel* pixels;

    // skala szarości
    Pixel* grayscale_pixels;
    i8* grayscale;
    i8* local_grayscale;
    i8* grayscale_in_RGB;

    // histogram
    int histogram[256] = {0};
    int local_histogram[256] = {0};

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
    double sobel_t1, sobel_t2, median_t1, median_t2, histogram_t1, histogram_t2;

    // inicjalizacja MPI
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Proces %d !!!\n", rank);

    // główny proces wykonuje przerobienie zdjęcia na odpowiednie struktury, 
    // konwersję do skali szarości oraz przygotowanie nazw plików docelowych
    if(rank == 0) {
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
    }

    // wszystkie procesy czekają aż załaduje się zdjęcie
    MPI_Barrier(MPI_COMM_WORLD);

    // kopiowanie wysokości i szerokości obrazka do innych procesów
    // MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_start = rank * ((width * height) / size);
    local_end = (rank == size - 1) ? (width * height) : (rank + 1) * ((width * height) / size);

    // kopiowanie obrazka do innych procesów
    // MPI_Bcast(local_grayscale, local_end - local_start, MPI_UINT8_T, 0, MPI_COMM_WORLD);

    

    
    
    // obliczenie histogramu
    histogram_t1 = MPI_Wtime();
    // for (int i = local_start; i < local_end; i++) {
    //     local_histogram[grayscale[i]]++;
    // }

    // MPI_Reduce(local_histogram, histogram, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    histogram_t2 = MPI_Wtime();

    // wypisanie histogramu
    if (rank == 0) {
        for(int i = 0; i < 256; i++) {
            printf("%d. Natężenie: %d\n", i, histogram[i]);
        }
    }
    

    // główny proces zwalnia miejsce ze zmiennych
    if(rank == 0) {
        // zwalnianie pamięci
        stbi_image_free(rgb_image);
        free(grayscale);
        free(pixels);
        free(grayscale_pixels);
        free(grayscale_in_RGB);
    }
    free(local_grayscale);

    MPI_Finalize();
    return 0;
}