#!/bin/bash
#SBATCH --job-name="sim_beehive"
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=128
#SBATCH --time=05:00:00
#SBATCH --partition=rome

module purge
module load 2022
module load GCCcore/11.3.0
echo "OpenMP parallelism"

for ncores in 1 2 4 8 16 32 64 128
do
echo "Running with OMP_NUM_THREADS = $ncores"
export OMP_NUM_THREADS=$ncores
echo "CPUS: " $OMP_NUM_THREADS
srun --ntasks=1 --cpus-per-task=$ncores ./sim_beehive
echo "Done with $ncores threads."
done