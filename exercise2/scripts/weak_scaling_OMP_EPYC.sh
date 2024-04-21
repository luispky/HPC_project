#!/bin/bash
#SBATCH --job-name=ex2EwOMP          
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

# OpenMP Weak scaling experiment for the Mandelbrot set
# The number of OpenMP threads and the problem size are increased proportionally
# The ratio of the problem size to the number of OpenMP threads is kept constant
# The number of MPI processes is kept constant at 1

# Executable
executable="../build/bin/mandelbrot"

# Output directory
output_dir="../results/"

# Output file 
output_file="${output_dir}weak_scaling_OMP_EPYC.csv"

# List of number of workers
lst1=(1 2 4 8)
lst2=({16..64..8})
# merge the two lists
number_workers=("${lst1[@]}" "${lst2[@]}")

# Constant amout of work per worker: C = problem size / number of workers
# Therefore, problem size = C * number of workers 
C=1000000
# The default setting is such that C = 2MB/worker

# Add CSV header
echo "Workers,Size,Walltime(s)" >> ${output_file}

# Define the number of processes to use for MPI parallelism with OpenMP theads 
processes=1

# Loop over the number of workers
# For weak scaling we keep the amount of work per worker constant
# and increase the number of workers
# Since the number of workers is proportional to the problem size
# increasing the number of workers will increase the problem size
for workers in "${number_workers[@]}"; do
  # Calculate the x and y dimensions of the image, nx=ny=n
  n=$(echo "sqrt($workers * $C)" | bc -l | xargs printf "%.0f")
  # Set the number of OpenMP threads to the number of workers
  threads=$workers
  # Run MPI command with specified parameters and append results to the output file
  export OMP_PLACES=threads
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