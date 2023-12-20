#!/bin/bash
#SBATCH -n 2
#SBATCH -c 6
mpiexec ./main_mpi ./median_test_2.png