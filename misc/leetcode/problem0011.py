# https://leetcode.com/problems/container-with-most-water/

class Solution(object):
    def maxArea(self, height):
        """
        :type height: List[int]
        :rtype: int
        """
        # solution method: sliding window of variable size
        
        maxArea = 0
        
        left = 0
        right = len(height)-1
        
        # slide window along to end, then increase/decrease size and reiterate
        while (left<right):
            maxArea = max(maxArea,min(height[left], height[right]) * (right-left))
            
            if (height[left]<height[right]):
                left+=1
            else:
                right-=1
        return maxArea