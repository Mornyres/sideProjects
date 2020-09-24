# https://leetcode.com/problems/trapping-rain-water/

class Solution(object):
    def trap(self, height):
        """
        :type height: List[int]
        :rtype: int
        """
        if (len(height)<3):
            return 0
        
        water = 0
        # go left to right
        left = 0
        right = 1
        heightCount=0
        maxR = 0
                
        while (left<right and left<(len(height)-1)):
            # try to find a pair for left
            if (right>=len(height)):
                heightCount=0
                height[left]=maxR
                right=left+1
                

                
            elif (height[right]>=height[left]):
                water+= (height[left]) * (right-left-1)
                water-=heightCount
                
                heightCount=0
                left=right
                right+=1
                maxR = 0

            else:
                maxR=max(height[right],maxR)
                heightCount+=height[right]
                right+=1

                
        return water