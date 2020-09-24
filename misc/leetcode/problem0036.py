# https://leetcode.com/problems/valid-sudoku/
class Solution(object):
    def isValidSudoku(self, board):
        """
        :type board: List[List[str]]
        :rtype: bool
        """
        return self.rSolve(board,0)
        
    def rSolve(self,board,col):
        # row by row, mark each square as valid, backtrack
        if (col >= 9):
            return True
        for row in range(9):
            if (self.validate(board,row,col) == False):
                return False
        return (self.rSolve(board,col+1))
        
    def validate(self,board,sRow,sCol): 
        if (board[sRow][sCol] == "."):
            return True
        print("testing:", board[sRow][sCol], sRow, sCol)
        for i in range(9): 
            if((board[sRow][i] == board[sRow][sCol]) and i != sCol):
                print("bad row")
                return False
        for i in range(9): 
            if((board[i][sCol] == board[sRow][sCol]) and i != sRow): 
                print("bad col")
                return False
        for i in range(3): 
            for j in range(3):
                squareRow = i+(sRow-sRow%3)
                squareCol = j+(sCol-sCol%3)
                if((board[squareRow][squareCol] == board[sRow][sCol]) and [squareRow,squareCol] != [sRow,sCol]): 
                    print("bad square")
                    return False
        print("valid!")
        return True
        