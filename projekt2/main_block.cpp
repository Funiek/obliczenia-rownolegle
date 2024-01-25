#include <stdio.h>
#include <iostream>
#include <chrono>
#include <random>
#include <cstring>

#define tp std::chrono::high_resolution_clock::time_point
#define ITERATIONS 10

double get_rand() {
    return 1.0 + (std::rand() / (float)RAND_MAX) * 5;
}

void execute(double* vecA, double* vecB, double* vecC, int N) {
    int x = 1;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) {
                vecC[i * N + j] += vecA[i * N + k] * vecB[k * N + j]; 
            }
        }
    }
}

void execute_2(double* vecA, double* vecB, double* vecC, int N, int iter_size_i, int iter_size_j, int iter_size_k) {
    for(int i = 0; i < iter_size_i; i++) {
        for(int j = 0; j < iter_size_j; j++) {
            for(int k = 0; k < iter_size_k; k++) {
                vecC[i * N + j] += vecA[i * N + k] * vecB[k * N + j]; 
            }
        }
    }
}

void block_matrix_multiplication(double* vecA, double* vecB, double* vecC, int N, int K) {
    for(int i = 0; i < N; i += K) {
        for(int j = 0; j < N; j += K) {
            for(int k = 0; k < N; k += K) {
                double* sub_A = &vecA[i * N + k];
                double* sub_B = &vecB[k * N + j];
                double* sub_C = &vecC[i * N + j];

                int iter_size_i = (i + K <= N) ? K : (N - i);
                int iter_size_j = (j + K <= N) ? K : (N - j);
                int iter_size_k = (k + K <= N) ? K : (N - k);

                execute_2(sub_A, sub_B, sub_C, N, iter_size_i, iter_size_j, iter_size_k);
            }
        }
    }
}

bool diff(double* arr1, double* arr2, int N) {
    for(int i = 0; i < N; i++) {
        if(arr1[i] != arr2[i]) {
            return true;
        }
    }

    return false;
}

int main(int argc, char** argv) {
    srand((unsigned) time(NULL));
    int N = atoi(argv[1]);
    int K = atoi(argv[2]);
    double* vecA_seq = new double[N*N];
    double* vecB_seq = new double[N*N];
    double* vecC_seq = new double[N*N];
    double* vecA = new double[N*N];
    double* vecB = new double[N*N];
    double* vecC = new double[N*N];
    

    for(int i = 0; i < N*N; i++) {
        vecA[i] = get_rand();
        vecB[i] = get_rand();
        vecC[i] = 0;
        vecA_seq[i] = vecA[i];
        vecB_seq[i] = vecB[i];
        vecC_seq[i] = vecC[i];
    }

    execute(vecA_seq, vecB_seq, vecC_seq, N);

    tp t1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++)
    {
        memset(vecC,0,sizeof(double)*N*N);
        block_matrix_multiplication(vecA, vecB, vecC, N, K);
    }

    tp t2 = std::chrono::high_resolution_clock::now();

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

    // std::cout << "\n";

    // for(int i = 0; i < N; i++) {
    //     for(int j = 0; j < N ; j++) {
    //         std::cout << vecC_seq[i * N + j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    if(diff(vecC, vecC_seq, N)) {
        printf("\nNIEZGODNE!!!\n");
    }
    else {
        printf("\nZGODNE!!!\n");
    }

    printf("time (s): %lf\n", std::chrono::duration<double>(t2-t1).count()/ITERATIONS);

    delete[] vecA;
    delete[] vecB;
    delete[] vecC;
    delete[] vecA_seq;
    delete[] vecB_seq;
    delete[] vecC_seq;
}