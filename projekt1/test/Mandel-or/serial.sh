#!/bin/bash

#SBATCH -n 1
#SBATCH -c 8
#SBATCH -o serial.out
#SBATCH -e serial.err

./serial

