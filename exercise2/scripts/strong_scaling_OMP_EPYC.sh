#!/bin/bash
#SBATCH --job-name=ex2EsOMP          
#SBATCH --nodes=1
#SBATCH --ntasks=1       
#SBATCH --cpus-per-task=64
#SBATCH --output=output_epyc.%j.out       
#SBATCH --error=error_epyc.%j.err         
#SBATCH --time=02:00:00
#SBATCH --partition=EPYC
#SBATCH --nodelist=epyc003
#SBATCH --exclusive

module load openMPI/4.1.5/gnu/12.2.1

# OpenMP Strong scaling experiment for the Mandelbrot set
# The problem size is kept constant
# The number of OpenMP threads increases and
# the number of MPI processes is kept constant at 1

# Executable
executable="../build/bin/mandelbrot"

# Output directory
output_dir="../results/"

# Output file 
output_file="${output_dir}strong_scaling_OMP_EPYC-cores.csv"

# List of number of workers
number_workers=({1..64})

# Dimensions of the Mandelbrolt set image: nx=ny=n
n=1000

# Add CSV header
echo "Workers,Size,Walltime(s)" >> ${output_file}

# Define the number of processes to use for MPI parallelism with OpenMP theads 
processes=1

# Loop over the number of workers
for workers in "${number_workers[@]}"; do
  # Set the number of MPI processes to the number of workers
  threads=$workers
  # Run MPI command with specified parameters and append results to the output file
  export OMP_PLACES=cores
  export OMP_PROC_BIND=close
  result=$(mpirun -np ${processes} \
    --map-by socket --bind-to socket \
    ${executable} ${threads} ${n} ${n})
  tail -n 1 <<< "$result" | awk "{print \"${workers},${n},\" \$4}" >> ${output_file}
done

# Store the job ID
job_id=$SLURM_JOB_ID

# Run sacct to retrieve job statistics and print to standard output
echo "Job Statistics for Job ID $job_id:"
sacct -j $job_id --format=JobID,JobName,Partition,MaxRSS,MaxVMSize,Elapsed,State