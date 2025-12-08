import sys
from collections import deque
def solve(s: int):
    n = len(s)
    q = deque([s, 0])
    seen = set(s)
    
    def helper(n, pos):
        length = len(str(n))
        val = (n + 10**(length - 1 - pos)) % 10**length
        return f"{val:0{length}d}"  
    print(helper(2998, 2))
    print(helper(2998, 2))
    print(helper(9999, 3))

    # while q:
    #     val, steps = q.popleft()
    #     if string == string[::-1]:
    #         return steps
        
    #     for i in range(n):
    #         # skip if its already equal to complement
    #         if val[i] == val[n - i + 1]:
    #             continue
    #         new_val = helper(val, i)
    #         if new_val not in seen:
    #             seen.add(new_val)
    #             q.append((new_val, steps + 1))
            
    # return -1

def main():
    data = sys.stdin.read().split()
    s = data[0]
    result = solve(s)
    if result is not None:
        print(result)

if __name__ == "__main__":
    main()
