#!/usr/bin/env python3

from itertools import combinations
from itertools import permutations

C = [ 
     ((0,0), [1,3,2]),
     ((0,1), [3,1,2]),
     ((1,0), [-1,3,2]),
     ((1,1), [-3,1,2]),
]

C = [ 
     ((0,0), [1,2]),
     ((0,1), [2,1]),
     ((1,0), [-1,2]),
     ((1,1), [-2,1]),
]

for (j,i) in C:
    print(f"{j} ", end="")
    b = 0
    for ii in i:
        if ii < 0:
            b = 1 - b
        print(str(b), end="")
        if abs(ii) == 2:
            b = 1 - b
    print("")

