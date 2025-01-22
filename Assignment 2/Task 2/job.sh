#!/bin/bash 
#SBATCH --job-name="pi" 
#SBATCH --nodes=1
#SBATCH --ntasks=1 
#SBATCH --cpus-per-task=32
#SBATCH --time=00:10:00 
#SBATCH --partition=rome 
#SBATCH --output=pi_%j.out 
#SBATCH --error=pi_%j.err 
module purge 
module load 2022 
module load GCCcore/11.3.0 
echo "OpenMP parallelism" 
echo
export OMP_NUM_THREADS=32 
for niter in 31250000 62500000 125000000 250000000 500000000 1000000000 2000000000
do 
export NITER=$niter
echo "ITERATIONS: " $NITER
echo "ITERATIONS: " $NITER >&2 
./pi 
echo "DONE " 
done