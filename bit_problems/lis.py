class BIT:
    def __init__(self, arr):
        self.n = len(arr) + 1
        self.tree = [0] * self.n

    def update(self, i, val):
        while i < self.n:
            self.tree[i] = max(self.tree[i], val)
            i += i & -i

    def query(self, i):
        ans = 0
        while i > 0:
            ans = max(ans, self.tree[i])
            i -= i & -i
        return ans
    
class Solution:
    def lengthOfLIS(self, nums: List[int]) -> int:
        b = sorted(set(nums))
        mappings = {b[i - 1]: i for i in range(1, len(b) + 1)}
        comp_indexes = [mappings[v] for v in nums]
        bit = BIT(nums)

        for i, v in enumerate(nums):
            bit.update(comp_indexes[i], bit.query(comp_indexes[i] - 1) + 1)

        return max(bit.tree)