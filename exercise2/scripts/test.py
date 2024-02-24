ny = 825
nx = 800
sizes = range(3, 6)
for size in sizes:
  print(f"size: {size}")  
  recvcounts = [0] * size
  displs = [0] * size
  boundaries = [0] * 4
  for rank in range(size):
    local_ny = ny // size
    reminder = ny % size
    local_ny_start = local_ny * rank + min(rank, reminder)
    local_ny_stop = local_ny_start + local_ny + (1 if rank < reminder else 0)
    recvcounts[rank] = (local_ny_stop - local_ny_start)*nx
    displs[rank] = (local_ny_start*nx)
    boundaries[0] = local_ny_start*nx + 0
    boundaries[1] = local_ny_stop*nx + 0
    boundaries[2] = local_ny_start*nx + nx
    boundaries[3] = local_ny_stop*nx + nx
    print(f"rank: {rank}, local_ny: {local_ny}, local_ny_start: {local_ny_start}, local_ny_stop: {local_ny_stop}, local_ny_size: {local_ny_stop - local_ny_start}")
    # Print rank and boundaries
    print(f"rank: {rank}, boundaries: {boundaries}")
  print(f"recvcounts: {recvcounts}")
  # Sum of recvcounts and product of nx and ny
  print(f"sum(recvcounts): {sum(recvcounts)}")
  print(f"nx*ny: {nx*ny}")
  print(f"displs: {displs}")
  print()
