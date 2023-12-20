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
    int local_start, local_end;
    i8* rgb_image = NULL;
    Pixel* pixels = NULL;

    // skala szarości
    Pixel* grayscale_pixels = NULL;
    i8* grayscale = NULL;
    i8* grayscale_in_RGB = NULL;

    // histogram
    int histogram[256] = {0};
    int local_histogram[256] = {0};

    // sobel
    i8* sobel_operator_result = NULL;
    i8* local_sobel_operator_result = NULL;
    i8* local_sobel_normalize_result = NULL;
    i8* sobel = NULL;

    // mediana
    i8* image_median_result = NULL;
    i8* local_image_median_result = NULL;
    i8* image_median = NULL;

    // mpi
    int *recv_counts = NULL;
    int *displacements = NULL;
    int *rank_intervals = NULL;
    int interval;
    
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
    double sobel_t1, sobel_t2, median_t1, median_t2, histogram_t1, histogram_t2, sobel_diff, median_diff, histogram_diff, sobel_global, median_global, histogram_global;

    // inicjalizacja MPI
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
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

        // inicjalizacja tabel dla głównego procesu
        image_median_result = (i8*)malloc(width * height * sizeof(i8));
        sobel_operator_result = (i8*)malloc(width * height * sizeof(i8));
    }
    rank_intervals = (int*)malloc(size * sizeof(int));

    // wszystkie procesy czekają aż załaduje się zdjęcie
    MPI_Barrier(MPI_COMM_WORLD);

    // kopiowanie wysokości i szerokości obrazka do innych procesów
    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    local_start = rank * ((width * height) / size);
    local_end = (rank == size - 1) ? (width * height) : (rank + 1) * ((width * height) / size);
    interval = local_end - local_start;
    printf("id: %d w: %d h: %d start: %d end: %d\n", rank, width, height, local_start, local_end);
    MPI_Gather(&interval, 1, MPI_INT, rank_intervals, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(rank_intervals, size, MPI_INT, 0, MPI_COMM_WORLD);

    recv_counts = (int*)malloc(size * sizeof(int));
    displacements = (int*)malloc(size * sizeof(int));

    // inicjalizacja recv_counts i displacements
    for (int i = 0; i < size; i++) {
        recv_counts[i] = rank_intervals[i];  // każdy proces wysyła określoną liczbę danych
        displacements[i] = (i==0) ? 0 : displacements[i - 1] + rank_intervals[i - 1];  // przesunięcie w buforze dla każdego procesu

        // printf("i:%d recv_counts:%d displacements:%d\n", i, recv_counts[i], displacements[i]);
    }
    interval = recv_counts[rank];

    // kopiowanie obrazka do innych procesów
    if(rank != 0) {
        grayscale = (i8*)malloc(width * height * sizeof(i8));
    }
    MPI_Bcast(grayscale, width * height, MPI_UINT8_T, 0, MPI_COMM_WORLD);

    // obliczenie histogramu
    histogram_t1 = MPI_Wtime();
    for (int i = local_start; i < local_end; i++) {
        local_histogram[grayscale[i]]++;
    }
    MPI_Reduce(local_histogram, histogram, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    histogram_t2 = MPI_Wtime();

    // wypisanie histogramu
    if (rank == 0) {
        for(int i = 0; i < 256; i++) {
            printf("%d. Natężenie: %d\n", i, histogram[i]);
        }
    }

    // czekamy aż procesy wszystkie będą gotowe na wykonanie operacji medianowej
    MPI_Barrier(MPI_COMM_WORLD);

    // zastosowanie filtru medianowego (nowa tablica wynikowa)
    median_t1 = MPI_Wtime();
    local_image_median_result = median(grayscale, width, height, local_start, local_end, rank);
    MPI_Gatherv(local_image_median_result, interval, MPI_UINT8_T, image_median_result, recv_counts, displacements, MPI_UINT8_T, 0, MPI_COMM_WORLD);
    median_t2 = MPI_Wtime();

    // zastosowanie filtru medianowego (nowa tablica wynikowa)
    sobel_t1 = MPI_Wtime();
    local_sobel_operator_result = sobel_operator(grayscale, width, height, local_start, local_end, rank);
    local_sobel_normalize_result = sobel_normalize(local_sobel_operator_result, local_start, local_end, rank);
    MPI_Gatherv(local_sobel_normalize_result, interval, MPI_UINT8_T, sobel_operator_result, recv_counts, displacements, MPI_UINT8_T, 0, MPI_COMM_WORLD);
    sobel_t2 = MPI_Wtime();

    // printf("Proces %d otrzymał dane:\n", rank);
    // for (int i = 0; i < interval; i++) {
        // printf("rank:%d data:%d\n", rank, grayscale[i]);
    // }
    // printf("\n");

    // liczenie i sumowanie czasow
    sobel_diff = sobel_t2 - sobel_t1;
    median_diff = median_t2 - median_t1;
    histogram_diff = histogram_t2 - histogram_t1;

    // przegadaj z Maćkiem czy to ma sens czy lepiej suma / size
    // MPI_Reduce(&sobel_diff, &sobel_global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sobel_diff, &sobel_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&median_diff, &median_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&histogram_diff, &histogram_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        // wypisanie czasow
        printf("Czasy:\nHistogram: %f\nMedian: %f\nSobel: %f\n", (double) histogram_global / size, (double) median_global / size, (double) sobel_global / size);

        // zapis zdjęcia w skali szarości do pliku
        grayscale_in_RGB = convert_gray_to_colors_array(grayscale, width, height, CHANNELS);
        save_image_png(image_path_gray, grayscale_in_RGB, width, height);

        // zapis zdjęcia z zastosowanym filtrem medianowym
        image_median = convert_gray_to_colors_array(image_median_result, width, height, CHANNELS);
        save_image_png(image_path_median, image_median, width, height);

        // zapis zdjęcia po filtrze z operatorem sobela do pliku
        sobel = convert_gray_to_colors_array(sobel_operator_result, width, height, CHANNELS);
        save_image_png(image_path_sobel, sobel, width, height);
    }
    
    // główny proces zwalnia miejsce ze zmiennych i zapisuje zdjęcia
    if(rank == 0) {
        // zwalnianie pamięci
        stbi_image_free(rgb_image);
        free(pixels);
        free(grayscale_pixels);
        free(grayscale_in_RGB);
        free(image_median);
        free(image_median_result);
        free(sobel_operator_result);
        free(sobel);
    }
    // zwalnianie pamięci
    free(grayscale);
    free(recv_counts);
    free(displacements);
    free(rank_intervals);
    free(local_image_median_result);
    free(local_sobel_operator_result);
    free(local_sobel_normalize_result);

    MPI_Finalize();
    return 0;
}