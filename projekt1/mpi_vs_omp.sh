#!/bin/bash
mpirun -np 1 ./main_mpi test_10000.png >> mpivsomp/mpi_1.txt
mpirun -np 2 ./main_mpi test_10000.png >> mpivsomp/mpi_2.txt
mpirun -np 4 ./main_mpi test_10000.png >> mpivsomp/mpi_4.txt
mpirun -np 6 ./main_mpi test_10000.png >> mpivsomp/mpi_6.txt

export OMP_NUM_THREADS=1; mpirun -np 1 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_1_1.txt
export OMP_NUM_THREADS=1; mpirun -np 2 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_1_2.txt
export OMP_NUM_THREADS=1; mpirun -np 4 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_1_4.txt
export OMP_NUM_THREADS=1; mpirun -np 6 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_1_6.txt

export OMP_NUM_THREADS=2; mpirun -np 1 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_2_1.txt
export OMP_NUM_THREADS=2; mpirun -np 2 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_2_2.txt
export OMP_NUM_THREADS=2; mpirun -np 4 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_2_4.txt
export OMP_NUM_THREADS=2; mpirun -np 6 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_2_6.txt

export OMP_NUM_THREADS=4; mpirun -np 1 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_4_1.txt
export OMP_NUM_THREADS=4; mpirun -np 2 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_4_2.txt
export OMP_NUM_THREADS=4; mpirun -np 4 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_4_4.txt
export OMP_NUM_THREADS=4; mpirun -np 6 ./main_mpi_omp test_10000.png >> mpivsomp/mpi_4_6.txt
