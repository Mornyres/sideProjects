/* The following program generates a rectangular map (with zero-based indices).
   Given an initial starting point, find the point with the lowest altitude that can
   be reached under these conditions:
   1) moving adjacent only sideways, up, or down (no diagonals)
   2) moving only when the altitude of the neighboring location is equal
      to or less than that of the current location

   If there are multiple points with the same lowest altitude that can be reached, select
   the path that encounters the steepest drop at the point of divergence. If each of the
   paths start out with the same drop in altitude, compare the drop at the next step of
   each route (and if those are the same, continue comparing until a difference is found).
   If there is more than one path with the exact same ending altitude and same drops
   along the way, favor points whose row is closest to R0; with points along the same row,
   favor points whose columns are closest to C0.

Sample map:

      C0  C1  C2  C3
  R0  67  72  93   5
  R1  38  53  71  48
  R2  64  56  52  44
  R3  44  51  57  49

Starting at R0, C1 should end up at R1, C0.
Starting at R1, C3 should end up at R0, C3.
Starting at R3, C2 should end up at R2, C3 since the drop from 57 to 49 is steeper than the drop from 57 to 51 when comparing R3, C0 to R2, C3.

Implement printLowestPoint to correctly print the answer.

*/

import java.util.*;


public class LowPointFinder {
    public static class Map {
        // do not change anything in the Map class
        private int mGrid[][] = null;
        public int getNumRows() { return mGrid.length; }
        public int getNumColumns() { return mGrid[0].length; }
        public int getAltitude(int iRow, int iColumn) {
            return mGrid[iRow][iColumn];
        }
        public void printMap() {
            StringBuilder sbRow = new StringBuilder("    ");
            for (int i = 0; i < mGrid[0].length; i++) {
                String strCell = String.format("%4s", "C"+i);
                sbRow.append(strCell);
            }
            System.out.println(sbRow.toString());
            for (int i = 0; i < mGrid.length; i++) {
                String strCell = String.format("%4s", "R"+i);
                sbRow = new StringBuilder(strCell);
                for (int j = 0; j < mGrid[0].length; j++) {
                    strCell = String.format("%4d", getAltitude(i, j));
                    sbRow.append(strCell);
                }
                System.out.println(sbRow.toString());
            }
        }
        private int changeAltitude(int iAltitude, Random random) {
            return iAltitude + random.nextInt(11) - 5;
        }
        public Map(int iNumRows, int iNumColumns, int iRandomSeed) {
            mGrid = new int[iNumRows][iNumColumns];
            Random random = new Random(iRandomSeed);
            for (int i = 0; i < iNumRows; i++) {
                for (int j = 0; j < iNumColumns; j++) {
                    int iAltitude = 0;
                    if (i == 0) {
                        iAltitude = random.nextInt(101);
                    } else {
                        int iSideAltitude = 0;
                        if (j == 0) iSideAltitude = random.nextInt(101);
                        else iSideAltitude = getAltitude(i, j-1);
                        int iTopAltitude = getAltitude(i-1, j);
                        iAltitude = (changeAltitude(iSideAltitude, random)
                                     + changeAltitude(iTopAltitude, random))/2;
                        if (iAltitude < 0) iAltitude = 0;
                        else if (iAltitude > 100) iAltitude = 100;
                    }
                    mGrid[i][j] = iAltitude;
                }
            }
        }
    }

    public static int[][] findLowestPoint(Map map, int iRow, int iColumn, HashMap<String, int[][]> memo) {
        
        int steepness = 0;
        
        // Initialization
        // I want a dynamically sized data structure that acts like a stack, so I'll use a deque
        Deque<int[]> candidates = new ArrayDeque<int[]>(); 
        // I want a dynamically sized data structure that can support indexing, so a LinkedList seems like a good option
        LinkedList<int[][]> pathValues = new LinkedList<int[][]>();
        
        int[][] retValues = { {0}, {0}, {0, 0} };        
        int[][] parentValues = {{map.getAltitude(iRow,iColumn)}, {steepness}, {iRow,iColumn}};
        
        // The orthogonal coordinates relative to iRow, iColumn. Because of the order, this will naturally follow the desired order of row and column proximity.
        int[][] orthogonals = {{iRow-1,iColumn},{iRow+1,iColumn},{iRow,iColumn-1},{iRow,iColumn+1}};
        
        
        
        // Step 1: Determine viable neighbors to be visited
        for (int[] node: orthogonals) {
            if ((node[0] < map.getNumRows()) &&
                (node[0] >= 0) &&
                (node[1] < map.getNumColumns()) &&
                (node[1] >= 0) &&
                (map.getAltitude(node[0], node[1]) <= parentValues[0][0])) {
                // a neighbor is a viable candidate if it doesn't lie off the end of the array and has a lower altitude than the parent
                candidates.addFirst(node);
            }
        }
        
        // Step 2: Calculate/get candidate values, update parent values
        
        // Add the option of just staying still
        pathValues.add(parentValues);
        int oldAlt = parentValues[0][0];
        int[][] newParentValues = parentValues;
        
        memo.put(Arrays.toString(parentValues[2]), parentValues);
        
        // Pop candidates stack until empty
        while(candidates.peek() != null){
            int[] newParent = candidates.pop();
            steepness = Math.abs(oldAlt - map.getAltitude(newParent[0],newParent[1]));
            // update from existing memo table if possible; otherwise, recur with new values and then update memo
            if (memo.containsKey(Arrays.toString(newParent)) == false){
                newParentValues = findLowestPoint(map, newParent[0],newParent[1], memo);
                memo.put(Arrays.toString(newParent), newParentValues);
            } else {
                newParentValues = memo.get(Arrays.toString(newParent));
            }
            int[][] finalValue = {{newParentValues[0][0]},{steepness},{newParentValues[2][0], newParentValues[2][1]}};
            pathValues.add(newParentValues);
        }
        
        // Step 3: Determine return value
        int retIndex = 0;
        int min_alt = parentValues[0][0];
        int max_steepness = parentValues[1][0];
        
        // iterate over possible paths for this parent node
        for (int i = 0; i < pathValues.size(); i++) {

            int[][] iValues = pathValues.get(i);
            // immediately ignore this path if the altitude is higher than the current minimum
            if (iValues[0][0] > min_alt) {
                continue;
            // consider steepness as second factor if altitude is equal
            } else if (iValues[0][0] == min_alt) {
                if (iValues[1][0] < max_steepness) {
                    continue;
                }
                max_steepness = iValues[1][0];
                
            // immediately prefer this path if altitude is lower than current min
            } else {
                min_alt = iValues[0][0];
                max_steepness = iValues[1][0];
            }
            retIndex = i;

        }
        
        // get the ideal path values to return
        retValues = pathValues.get(retIndex);

        return retValues;
    }
    
    public static void printLowestPoint(Map map, int iRow, int iColumn) {
        HashMap<String, int[][]> myMemo = new HashMap<String, int[][]>();
        int [][] result = findLowestPoint(map, iRow, iColumn, myMemo);
        
        System.out.println("The lowest reachable point occurs at "
                            + result[2][0]
                            + ", "
                            + result[2][1]
                            + " with an altitude of "
                            + result[0][0]);
    }
    
    public static void main(String args[]) {
        Map map = new Map(10, 10, 0);
        map.printMap();
        printLowestPoint(map, 1, 9);
    }
}
