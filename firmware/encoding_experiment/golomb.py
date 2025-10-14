#!/usr/bin/env python3

from itertools import combinations
from itertools import permutations

G = [0,1,3]
m = 7
print(f"G={G}")
r = []
for j in G:
    i = 0
    if i == j:
        continue
    x = j - i
    y = m - j
    s = x + y
    barf = f"{i=} {j=} | {s=} | {x=} {y=}"
    r.append((s, barf))

for (i,j,k) in combinations(G, 3):
    if i == m:
        continue
    w = j-i
    x = k-j
    y = m-k
    z = 0
    s = w + x + y + z
    barf = f"{i=} {j=} {k=} | {s=} | {w=} {x=} {y=} {z=}"
    r.append((s, barf))

r.sort(key=lambda x : x[0])

for (s,b) in r:
    print(b)

