#https://leetcode.com/problems/string-to-integer-atoi/

class Solution(object):
    def myAtoi(self, str):
        """
        :type str: str
        :rtype: int
        """
        # strip whitespace
        str = str.strip()
        
        print str
        numList = ""
        outputNum = 0
        
        negative = 0
        
        INT_MAX = 2147483647
        INT_MIN = -2147483648

        
        lastCharSymbol = 0
        # while the current character is numerical or a dash:
        for i, char in enumerate(str):
            ascii = ord(char)
            if (ascii == 45 and i==0):
                negative = 1
            elif (ascii == 43 and i==0):
                continue
            
            elif (48 <= ascii <= 57):
                numList = numList + char
                
            else:
                break

        if (numList == ""):
            return 0
        
        outputNum = int(numList)
        
        if (negative == 1):
            outputNum = 0-outputNum
        if (outputNum > INT_MAX):
            outputNum = INT_MAX
        if (outputNum < INT_MIN):
            outputNum = INT_MIN

        return outputNum