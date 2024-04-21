#!/bin/bash
#SBATCH --job-name=ex2EsMPI          
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --output=output_epyc.%j.out    
#SBATCH --error=error_epyc.%j.err      
#SBATCH --time=02:00:00
#SBATCH --partition=EPYC
#SBATCH --nodelist=epyc003,epyc004
#SBATCH --exclusive

module load openMPI/4.1.5/gnu/12.2.1

# MPI Strong scaling experiment for the Mandelbrot set
# The problem size is kept constant
# The number of MPI processes increases and
# the number of OpenMP threads is kept constant at 1

# Executable
executable="../build/bin/mandelbrot"
# executable="../bin/mandelbrot"

# Output directory
output_dir="../results/"

# Output file 
output_file="${output_dir}strong_scaling_MPI_EPYC-4nodes.csv"

# List of number of workers
lst=({2..256..1})
number_workers=("1" "${lst[@]}")

# Dimensions of the Mandelbrolt set image: nx=ny=n
n=1000

# Add CSV header
echo "Workers,Size,Walltime(s)" >> ${output_file}

# Define the number of threads to use for OpenMP parallelism within each MPI process
threads=1

# Loop over the number of workers
for workers in "${number_workers[@]}"; do
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