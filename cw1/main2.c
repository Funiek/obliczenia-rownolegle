#include "mpi.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


double calculate_whole_time(int number_of_bytes, int N){
    int myrank;
    void *x = (void *)malloc(number_of_bytes);
    void *y = (void *)malloc(number_of_bytes);

    MPI_Status status;


    // Pobierz numer procesu w globalnym komunikatorze
    MPI_Comm_rank(MPI_COMM_WORLD, & myrank);

    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    double startTime = MPI_Wtime();
    if (myrank == 0)
    {

        for (int i = 0; i < N; i++)
        {
            MPI_Send(x, number_of_bytes, MPI_BYTE, 1, 12, MPI_COMM_WORLD);
            MPI_Recv(y, number_of_bytes, MPI_BYTE, 1, 12, MPI_COMM_WORLD, &status);
        }
    }
    else if (myrank == 1)
    {
        for (int i = 0; i < N; i++)
        {
            MPI_Recv(x, number_of_bytes, MPI_BYTE, 0, 12, MPI_COMM_WORLD, &status);
            MPI_Send(y, number_of_bytes, MPI_BYTE, 0, 12, MPI_COMM_WORLD);
        }
    }
    double endTime = MPI_Wtime();
    
    free(x);
    free(y);

    double whole_time = endTime - startTime;
    return whole_time;
    }


double calculate_whole_time_a(int number_of_bytes, int N){
    int myrank;
    void *x = (void *)malloc(number_of_bytes);
    void *y = (void *)malloc(number_of_bytes);

    MPI_Status status;


    // Pobierz numer procesu w globalnym komunikatorze
    MPI_Comm_rank(MPI_COMM_WORLD, & myrank);

    // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    double startTime = MPI_Wtime();
    if (myrank == 0)
    {

        for (int i = 0; i < N; i++)
        {
            
            MPI_Request send_request, recv_request;
            MPI_Status status;

            MPI_Isend(x, number_of_bytes, MPI_BYTE, 1, 12, MPI_COMM_WORLD,&send_request);
            MPI_Irecv(y, number_of_bytes, MPI_BYTE, 1, 12, MPI_COMM_WORLD,&recv_request);


            MPI_Wait(&send_request, &status);
            MPI_Wait(&recv_request, &status);

        }
    }
    else if (myrank == 1)
    {
        for (int i = 0; i < N; i++)
        {
           
            MPI_Request send_request, recv_request;
            MPI_Status status;

            MPI_Irecv(y, number_of_bytes, MPI_BYTE, 0, 12, MPI_COMM_WORLD,&recv_request);
            MPI_Isend(x, number_of_bytes, MPI_BYTE, 0, 12, MPI_COMM_WORLD,&send_request);


            MPI_Wait(&recv_request, &status);
            MPI_Wait(&send_request, &status);
        }
    }
    double endTime = MPI_Wtime();
    
    free(x);
    free(y);

    double whole_time = endTime - startTime;
    return whole_time;
}


double calculate_time_of_operation(int number_of_bytes, int N, double whole_time)
{
    double operation_time = whole_time / (2 * N);

    return operation_time;
}

double calculate_speed(int number_of_bytes, int N, double whole_time )
{
    double operation_time =calculate_time_of_operation(number_of_bytes,N,whole_time);
    
    return number_of_bytes/operation_time;
}

int main(int argc, char **argv)
{
    int myrank;
    // int N = 10;
    // int number_of_bytes = 10;
    struct timespec time1, time2;

    // Inicjalizacja podsystemu MPI
    MPI_Init(&argc, &argv);


    // Pobierz numer procesu w globalnym komunikatorze
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    for(int number=1;number<1024*1024*8+1;number*=2){
        for(int cycles=1;cycles<=1024*32;cycles*=2)
        {
            double whole_time = calculate_whole_time(number, cycles);
            double speed = calculate_speed(number,cycles,whole_time);
            double time = calculate_time_of_operation(number,cycles,whole_time);

            double whole_time_a = calculate_whole_time_a(number, cycles);
            double speed_a = calculate_speed(number,cycles,whole_time_a);
            double time_a = calculate_time_of_operation(number,cycles,whole_time_a);

            if(myrank==1){
                printf("%d, %d, %f, %f, %f, %f, %f, %f\n", number,cycles,whole_time, speed,time,whole_time_a, speed_a,time_a);
            }
        }
    }

    MPI_Finalize();
    return 0;
}