#include <stdio.h>
#include <iostream>
#include <chrono>
#include <random>

#define tp std::chrono::high_resolution_clock::time_point

double get_rand() {
    return 1 + (std::rand() % 5);
}


void execute(double* vecA, double* vecB, double* vecC, int N) {
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) {
                vecC[i * N + j] += vecA[i * N + k] * vecB[k * N + j]; // dodawanie mnożenia wartości z wiersza z macierzy A z kolumną z macierzy B
            }
        }
    }
}

int main(int argc, char** argv) {
    srand((unsigned) time(NULL));
    int N = atoi(argv[1]);
    double* vecA = new double[N*N];
    double* vecB = new double[N*N];
    double* vecC = new double[N*N];
    

    for(int i = 0; i < N*N; i++) {
        vecA[i] = get_rand();
        vecB[i] = get_rand();
        vecC[i] = 0;
    }

    tp t1 = std::chrono::high_resolution_clock::now();

    execute(vecA, vecB, vecC, N);

    tp t2 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            std::cout << vecA[i * N + j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            std::cout << vecB[i * N + j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N ; j++) {
            std::cout << vecC[i * N + j] << " ";
        }
        std::cout << "\n";
    }

    printf("time (s): %lf\n", std::chrono::duration<double>(t2-t1).count());

    delete[] vecA;
    delete[] vecB;
    delete[] vecC;
}