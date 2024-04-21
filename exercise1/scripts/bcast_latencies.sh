#!/bin/bash
#SBATCH --job-name=ex1EPYCbc            
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128      
#SBATCH --output=output_epyc.%j.out
#SBATCH --error=error_epyc.%j.err  
#SBATCH --time=02:00:00
#SBATCH --partition=EPYC
#SBATCH --nodelist=epyc002,epyc004
#SBATCH --exclusive

# THIN: 24 tasks per node
# EPYC: 128 tasks per node

module load openMPI/4.1.5/gnu/12.2.1

# Output directory
output_dir="../results/"

# Output file for each algorithm and collective operation
output_file="${output_dir}bcastEPYC.csv"

# Broadcast algorithms:
# 0: ignore : baseline
# 1: basic linear
# 2: chain
# 5: binary tree
bcast_algorithms=(0 1 2 5)
# Collect operation
collective_operation="bcast"

# List of number of processors
# number_processors=({4..48..4}) # 12 in total
number_processors=(2 4 8 16 32 48 64 96 128 176 224 256) # 12 in total

# Add CSV header
echo "Algorithm,Processors,Size(bytes),Avg_Latency(us)" >> ${output_file}

# Iterations to reduce noise
iterations=10

for iteration in $(seq 1 $iterations); do
  # Iterate over each algorithm for the current collective operation
  for bcast_algorithm in "${bcast_algorithms[@]}"; do
      # Iterate over each number of processors
      for processors in "${number_processors[@]}"; do
        # Run MPI command with specified parameters and append results to the output file
        result=$(mpirun -np ${processors} \
          --map-by core \
          --mca coll_tuned_use_dynamic_rules true \
          --mca coll_tuned_${collective_operation}_algorithm ${bcast_algorithm} \
          ../bin/osu_${collective_operation} \
          -i 1e4 \
          -x 1e4)

        # Remove unnecessary lines, format, and append to CSV file
        tail -n +5 <<< "$result" | awk "{print \"${bcast_algorithm},${processors},\" \$1 \",\" \$2}" >> ${output_file}
      done
  done
done 

# ! Comments after backslash `\` are forbidden. Only the continuation of the code is allowed.
# * Command comments:
# result=$(mpirun -np ${processors} \  
#   --map-by core \  # Map processes by node, and allocate one core per process
#   --mca coll_tuned_use_dynamic_rules true \  # Enable dynamic tuning of collective communication algorithms
#   --mca coll_tuned_${collective_operation}_algorithm ${bcast_algorithm} \  # Specify the tuned algorithm for the current collective operation
#   ../bin/osu_${collective_operation} \  # Specify the executable for the current collective operation
#   -i 1e4 \  # Set the number of warm-up iterations
#   -x 1e4) # Set the number of total iterations