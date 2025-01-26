#!/bin/bash 
#SBATCH --partition=cbuild 
module load 2022 
module load GCCcore/11.3.0 
gcc -fopenmp -o sim_beehive sim_beehive.c -lm