#!/bin/bash

#SBATCH -n 16
#SBATCH -e parallel.err
#SBATCH -o parallel.out

mpiexec ./parallel