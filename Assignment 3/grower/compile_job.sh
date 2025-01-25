#!/bin/bash 
#SBATCH --partition=cbuild 
module load 2022 
module load GCCcore/11.3.0 
gcc -fopenmp -o sim_grower sim_grower.c -lm