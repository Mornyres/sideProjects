# https://leetcode.com/problems/majority-element/

class Solution(object):
    def majorityElement(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        n = len(nums)
        nums.sort()
        sum=0
        index=0
        val = nums[index]
        while (sum <= math.floor(n/2)):
            if (nums[index] == val):
                sum+=1
            else:
                val=nums[index]
            index+=1
        return val