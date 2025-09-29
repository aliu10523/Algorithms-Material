
# 8 
# [6, 5, 1, 12, 2, 4, 9, 8]
#  0 1 2 3 4 5 6 7
import sys

class BIT:
    def __init__(self, arr):
        self.n = len(arr) + 1
        self.tree = [0] * self.n

    def update(self, i, val):
        while i < self.n:
            self.tree[i] += val
            i += i & -i

    def query(self, i):
        ans = 0
        while i > 0:
            ans += self.tree[i]
            i -= i & -i
        return ans
    
def solve(arr):
    b = sorted(set(arr))
    mappings = {b[i - 1]: i for i in range(1, len(b) + 1)}
    comp_indexes = [mappings[v] for v in arr]
    bit = BIT(arr)
    res = 0
    for i in range(len(arr) - 1, -1, -1):
        res += bit.query(comp_indexes[i] - 1)
        bit.update(comp_indexes[i], 1)
    
    return res

def main():
    data = list(map(int, sys.stdin.read().split()))
    t = data[0]
    idx = 1
    for _ in range(t):
        n = data[idx]; idx += 1
        A = data[idx:idx+n]; idx += n
        res = solve(A)
        if res is not None:
            print(res)

if __name__ == "__main__":
    main()