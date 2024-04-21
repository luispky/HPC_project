module load openMPI/4.1.5/gnu/12.2.1

# Move to parent directory
cd ..
# Download and install the OSU Micro-Benchmarks library
wget https://mvapich.cse.ohio-state.edu/download/mvapich/osu-micro-benchmarks-7.3.tar.gz
tar -xvf osu-micro-benchmarks-7.3.tar.gz
rm -rf osu-micro-benchmarks-7.3.tar.gz 
cd osu-micro-benchmarks-7.3
./configure CC=/usr/bin/mpicc 
make
make install
# Create a bin directory and copy the compiled files to perform benchmarking of the broadcast and barrier operations
cd ../
mkdir bin
cp osu-micro-benchmarks-7.3/c/mpi/collective/blocking/osu_bcast bin/
cp osu-micro-benchmarks-7.3/c/mpi/collective/blocking/osu_barrier bin/
# Remove the osu-micro-benchmarks-7.3 directory
rm -rf osu-micro-benchmarks-7.3