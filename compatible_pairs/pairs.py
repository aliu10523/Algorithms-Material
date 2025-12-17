import sys
from collections import *
import heapq

nums = map(int, sys.stdin.read().split())
N = next(nums)
A = next(nums)
B = next(nums)
pairs = [(next(nums), next(nums)) for _ in range(N)]

id_to_quantity = {id : quantity for quantity, id in pairs}
unique_ids = set()
res = 0

for quantity, id in pairs:
    # if A / 2 == id or B / 2 == id:
    #     res += quantity // 2
    #     id_to_quantity[id] = 1 if quantity % 2 == 1 else 0
    #     unique_ids.add(id)
    #     continue

    unique_ids.add(id)

adj = defaultdict(list)
for node in unique_ids:
    compA = A - node
    compB = B - node
    # print(node, compA, compB)
    if compA in unique_ids:
        adj[node].append(compA)
    if A != B and compB in unique_ids:
        adj[node].append(compB)

# print(adj)
# print(id_to_quantity)
q = []
seen = set()
for id, neighbors in adj.items():
    if len(neighbors) == 1:
        q.append(id)

for node in q:
    if node in seen:
        continue
    loop = True
    while loop:
        changed = False
        seen.add(node)
        for neighbor in adj[node]:
            if neighbor == node:
                depletion = id_to_quantity[node] // 2
                res += depletion
                loop = False
                break
            if neighbor in seen:
                continue
            depletion = min(id_to_quantity[neighbor], id_to_quantity[node])
            id_to_quantity[neighbor] -= depletion
            id_to_quantity[node] -= depletion
            res += depletion
            node = neighbor
            changed = True
        if not changed:
            loop = False


print(res)