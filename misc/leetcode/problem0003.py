# https://leetcode.com/problems/longest-substring-without-repeating-characters/

class Solution(object):
    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        
        
        naive:
        for each character, check the chars on the right until you get a duplicate. save the resulting string and compare the lengths of all such strings.
        
        this will take (n-1)+(n-2)...+0 = n/2*(n-1)
        := O(n^2)
        
        can we optimize this somehow?
            start with size of set(s) as window size, go left to right 
            further optimization: 
                if windowsize not bigger than previous max uniques, return the latter
                
        this takes 
            (n-m)+(n-(m-1))... = O(mn) where m= uniques
         
        """
        if not s:
            return 0
        
        windowSize = len(set(s))
        if (windowSize <=2):
            return windowSize
        
        N = len(s)
        while (windowSize > 2):
            start = 0
            end = start+(windowSize-1)
            
            while (end<N):
                
                uniques = len(set(s[start:end+1]))
                #print("testing:",s[start:end+1])
                #print("uniques:",uniques)
                if (uniques == windowSize):
                    return windowSize
                start+=1
                end = start+(windowSize-1)
                
            windowSize-=1
            
        return windowSize