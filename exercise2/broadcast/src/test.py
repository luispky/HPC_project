import math
p = 8

"""
if id > 0  
    blocking_receive M
for (i := ceil(log_2(p)) - 1; i >= 0; i--)
    if (id % 2^(i+1) == 0 && id + 2^i <= p)
        send M to node id + 2^i
"""
for id in range(0, p):
  # print id number
  print(f"Node {id}:")
  number_of_iterations = math.ceil(math.log2(p))
  for i in range(number_of_iterations - 1, -1, -1):
      if id % 2 ** (i + 1) == 0 and id + 2 ** i <= p:
          print(f"send M to node {id + 2 ** i}")
  print("\n")


root = 5

# Reorder the nodes so that root is zero
# The new id of each node is (old_id - root) % p
# for id in range(0, p):
#   # print id number
#   print(f"Node {id}:")
#   number_of_iterations = math.ceil(math.log2(p))
#   for i in range(number_of_iterations - 1, -1, -1):
#       if (id - root) % p % 2 ** (i + 1) == 0 and (id - root) % p + 2 ** i <= p:
#           print(f"send M to node {(id - root) % p + 2 ** i}")
#   print("\n")

for rank in range(0, p):
  new_rank = (rank - root) % p
  print(f"Node {rank} -> Node {new_rank}:")
  number_of_iterations = math.ceil(math.log2(p))
  for i in range(number_of_iterations - 1, -1, -1):
      if new_rank % 2 ** (i + 1) == 0 and new_rank + 2 ** i <= p:
          print(f"send M to node {new_rank + 2 ** i}")
  print("\n")