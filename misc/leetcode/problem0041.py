# https://leetcode.com/problems/first-missing-positive/
class Solution(object):
    def firstMissingPositive(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        if (not nums):
            return 1
        
        return self.smallestMissing(nums, len(nums))

    def smallestMissing(self,a,k):
        buckets = [0] * (k+1)
        
        for i in a:
            if (i>=1 and i<=k):
                buckets[i] += 1

        
        for i in range(1,len(buckets)):
            
            if buckets[i] == 0:
                return i
            
        return i+1
        
        
        """
        Naive attempt based on modified count-sort. Good time complexity, but uses too much space
        
        if (not num):
            return 1
        
        return self.smallestMissing(nums, max(nums))

    def smallestMissing(self,a):
        buckets = [0] * (k+1)
        
        for i in a:
            if (i>0):
                buckets[i] += 1

        
        for i in range(1,len(buckets)):
            if buckets[i] == 0:
                return i
        return i+1
        """