#!/bin/bash
#SBATCH --job-name=ex2EwMPI          
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --output=output_epyc.%j.out       
#SBATCH --error=error_epyc.%j.err         
#SBATCH --time=02:00:00
#SBATCH --partition=EPYC
#SBATCH --nodelist=epyc003,epyc004
#SBATCH --exclusive

module load openMPI/4.1.5/gnu/12.2.1

# MPI Weak scaling experiment for the Mandelbrot set
# The number of MPI processes and the problem size are increased proportionally
# The ratio of the problem size to the number of MPI processes is kept constant
# The number of OpenMP threads is kept constant at 1

# Executable
executable="../build/bin/mandelbrot"
# executable="../bin/mandelbrot"

# Output directory
output_dir="../results/"

# Output file 
output_file="${output_dir}weak_scaling_MPI_EPYC.csv"

# List of number of workers
lst=(2 4 8 16 32 48 64 96 128 160 192 224 256)
number_workers=("1" "${lst[@]}")

# Constant amout of work per worker: C = problem size / number of workers
# Therefore, problem size = C * number of workers 
C=1000000
# The default setting is such that C = 2MB/worker

# Add CSV header
echo "Workers,Size,Walltime(s)" >> ${output_file}

# Define the number of threads to use for OpenMP parallelism within each MPI process
threads=1

# Loop over the number of workers
# For weak scaling we keep the amount of work per worker constant
# and increase the number of workers
# Since the number of workers is proportional to the problem size
# increasing the number of workers will increase the problem size
for workers in "${number_workers[@]}"; do
  # Calculate the x and y dimensions of the image, nx=ny=n
  n=$(echo "sqrt($workers * $C)" | bc -l | xargs printf "%.0f")
  # Set the number of MPI processes to the number of workers
  processes=$workers
  # Run MPI command with specified parameters and append results to the output file
  result=$(mpirun -np ${processes} \
    --map-by core \
    ${executable} ${threads} ${n} ${n})
  # Use awk to extract the Total Walltime value and append it to the output file
  tail -n 1 <<< "$result" | awk "{print \"${workers},${n},\" \$4}" >> ${output_file}
done

# Store the job ID
job_id=$SLURM_JOB_ID

# Run sacct to retrieve job statistics and print to standard output
echo "Job Statistics for Job ID $job_id:"
sacct -j $job_id --format=JobID,JobName,Partition,MaxRSS,MaxVMSize,Elapsed,State