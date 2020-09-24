# https://leetcode.com/problems/longest-palindromic-substring/

class Solution(object):
    def longestPalindrome(self, s):
        """
        :type s: str
        :rtype: str
        """
        # set window size N to 2
        # examine substring of size N
        # if the first N/2 chars match the last N/2 chars, set this substring as the longest and increment N
        # rinse and repeat

        rootSize = len(s)
        if (rootSize == 1):
            return str(s[0])
        if (rootSize == 0):
            return ""
        
        N = min(2,rootSize)
        lPal = str(s[0]) 
            
        while (N <= rootSize):
            start=0
            end = (N-1)
            while(end<rootSize):     
                # check substring from start to end
                substring = s[start:end+1]
                if (substring == substring[::-1]):
                    lPal = substring
                    break
                start+=1
                end = start + (N-1)  
            N+=1
                
        return lPal