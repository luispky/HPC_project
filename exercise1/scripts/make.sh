wget https://mvapich.cse.ohio-state.edu/download/mvapich/osu-micro-benchmarks-7.3.tar.gz
tar -xvf osu-micro-benchmarks-7.3.tar.gz
cd osu-micro-benchmarks-7.3
./configure CC=/usr/bin/mpicc 
make
make install

mkdir results bin plots stats

cd scripts/

cp ../osu-micro-benchmarks-7.3/c/mpi/collective/blocking/osu_bcast ../bin/
cp ../osu-micro-benchmarks-7.3/c/mpi/collective/blocking/osu_barrier ../bin/