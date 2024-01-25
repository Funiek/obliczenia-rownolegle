#include <stdio.h>
#include <iostream>
#include <chrono>
#include <random>
#include <omp.h>
#include <cstring>

#define tp std::chrono::high_resolution_clock::time_point
#define ITERATIONS 10

double get_rand() {
    return 1.0 + (std::rand() / (float)RAND_MAX) * 5;
}

void execute(double* vecA, double* vecB, double* vecC, int N) {
    #pragma omp parallel for schedule(guided) collapse(2)
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            double temp = 0.0;
            for(int k = 0; k < N; k++) {
                temp += vecA[i * N + k] * vecB[k * N + j];
            }
            vecC[i * N + j] = temp;
        }
    }
}

int main(int argc, char** argv) {
    omp_set_num_threads(16);
    
    srand((unsigned) time(NULL));
    int N = atoi(argv[1]);
    double* vecA = new double[N*N];
    double* vecB = new double[N*N];
    double* vecC = new double[N*N];
    

    for(int i = 0; i < N*N; i++) {
        vecA[i] = 1;
        vecB[i] = 1;
        vecC[i] = 0;
    }

    double t1 = omp_get_wtime();

    for (int i = 0; i < ITERATIONS; i++)
    {
        memset(vecC,0,sizeof(double)*N*N);
        execute(vecA, vecB, vecC, N);
    }

    double t2 = omp_get_wtime();

    // for(int i = 0; i < N; i++) {
    //     for(int j = 0; j < N; j++) {
    //         std::cout << vecA[i * N + j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // std::cout << "\n";

    // for(int i = 0; i < N; i++) {
    //     for(int j = 0; j < N; j++) {
    //         std::cout << vecB[i * N + j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    // std::cout << "\n";

    // for(int i = 0; i < N; i++) {
    //     for(int j = 0; j < N ; j++) {
    //         std::cout << vecC[i * N + j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    printf("time (s): %lf\n", (t2-t1)/ITERATIONS);

    delete[] vecA;
    delete[] vecB;
    delete[] vecC;
}