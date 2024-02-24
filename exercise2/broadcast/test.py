"""
Message M := [m_1, m_2, ..., m_n]
id = node number

for (i := 1; i <= n; i++) in parallel
    if (id != 0)
        blocking_receive m_i
        
    if (id != n)
        send m_i to node id + 1
"""

from mpi4py import MPI

def linear_pipeline_broadcast(comm, data, root=0):
    rank = comm.Get_rank()
    size = comm.Get_size()
    # reorganize the ranks so that the root is the first rank
    new_rank = (rank - root + size) % size 

    n = len(data)
    base_partition_size = 1000
    max_partitions = 10
    k = min(max_partitions, max(1, n // base_partition_size))
    partition_size = n // k

    start = 0
    end = 0

    for i in range(k-1):  
        start = i * partition_size
        end = start + partition_size
        if new_rank != 0:
            recv_data = comm.recv(source=new_rank-1, tag=0)
            data[start:end] = recv_data
        if new_rank != size-1:
          send_data = data[start:end]
          comm.send(send_data, dest=new_rank, tag=0)
    
    start = end
    if new_rank != 0:
        recv_data = comm.recv(source=new_rank-1, tag=0)
        data[start:] = recv_data
    # if new_rank != size-1:
    send_data = data[start:]
    comm.send(send_data, dest=new_rank, tag=0)

    return MPI.SUCCESS

def main():
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()
    data_size = 2001
    data = data_size*[0]
    root = 2
    if rank == root:
        print(f"Running with {size} processes")
        data = [i for i in range(data_size)]
    print(f"Rank {rank} starts with sum(data) = {sum(data)}")
    linear_pipeline_broadcast(comm, data, root)
    print(f"Rank {rank} ends with sum(data) {sum(data)}")

if __name__ == "__main__":
    main()

# mpirun -n 5 python test.py

# processors = 6
# print([(i-2+processors) % processors for i in range(0,processors)])
# print(min(10, max(1, 2001 // 1000)))