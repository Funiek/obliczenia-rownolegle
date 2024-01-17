#include <stdio.h>
#include <iostream>
#include <chrono>
#include <random>
#include <omp.h>

#define tp std::chrono::high_resolution_clock::time_point

double get_rand() {
    return 1 + (std::rand() % 5);
}


// void execute(double* vecA, double* vecB, double* vecC, int N) {
//     omp_set_num_threads(4);


//     #pragma omp parallel for schedule(guided) collapse(3)
//     for(int i = 0; i < N; i++) {
//         for(int j = 0; j < N; j++) {
//             for(int k = 0; k < N; k++) {
//                 vecC[i * N + j] += vecA[i * N + k] * vecB[k * N + j];
//             }
//         }
//     }
// }

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
    omp_set_num_threads(4);
    
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

    execute(vecA, vecB, vecC, N);

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

    printf("time (s): %lf\n", (t2-t1));

    delete[] vecA;
    delete[] vecB;
    delete[] vecC;
}