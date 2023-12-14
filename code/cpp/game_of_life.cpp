/*
Game of Life:

The Game of Life is a cellular automaton devised by Conway in 1970.

The board is made up of an m x n grid of cells, where each cell has an initial state: 
alive = 1
dead = 0

Each cell interacts with its eight neighbors (horizontal, vertical, diagonal) using the following rules:

Any live cell with fewer than 2 live neighbors dies as if caused by under-population.
Any live cell with 2 or 3 live neighbors lives on to the next generation.
Any live cell with more than 3 live neighbors dies, as if by over-population.
Any dead cell with exactly 3 live neighbors becomes a live cell, as if by reproduction.

Determine the next state of the game.

Follow up:
1. How to save space?
- If using Java, use byte[][].
- Don't make an extra board. Just rewrite the current one.
- All you need is a set of the cells that are alive
2. What if this can't fit into an array?
- Read the rows as streams from a disk, read 3 rows at a time
3. What if we just give you generating functions G(row, col, time) for the cell states?
- Just do the math with the generating functions.

*/

#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

void next_state_new_grid(std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        throw std::invalid_argument ("grid is empty lol\n");
    }
    int W = grid[0].size(); // get the size of first vector inside main vector
    int H = grid.size(); // because we conunt the number of vectors inside a vector
    std::vector<std::vector<int>> return_grid = grid;

    std::vector<std::pair<int, int>> neighbors = {
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    for (int row = 0; row < H; row++) {
        for (int col = 0; col < W; col ++) {
            int alive_neighbors = 0;
            // count live neighbors:
            for (const auto& neighbor : neighbors) {
                int new_row = row + neighbor.first;
                int new_col = col + neighbor.second;
                if ((new_col >= 0 && new_col < W) && (new_row >= 0 && new_row < H) && grid[new_row][new_col]) {
                    alive_neighbors++;
                }
            }
            if (grid[row][col]) {
                if (alive_neighbors<2 || alive_neighbors>3) {
                    return_grid[row][col] = 0;
                }
            } else {
                if (alive_neighbors == 3) {
                    return_grid[row][col] = 1;
                }
            }
        }
    }
    grid = return_grid;
}

void next_state_same_grid(std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        throw std::invalid_argument ("grid is empty lol\n");
    }
    int W = grid[0].size(); // get the size of first vector inside main vector
    int H = grid.size(); // because we conunt the number of vectors inside a vector

    /*  Use truth table and use mod 2 at the end
        Truth table

        Orig / New / State
         0   / 0   / 0
         1   / 0   / 1
         0   / 1   / 2
         1   / 1   / 3
    */
   // func next_state
   // func convert_state

    std::vector<std::pair<int, int>> neighbors = {
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    for (int row = 0; row < H; row++) {
        for (int col = 0; col < W; col ++) {
            int alive_neighbors = 0;
            // count live neighbors:
            for (const auto& neighbor : neighbors) {
                int new_row = row + neighbor.first;
                int new_col = col + neighbor.second;
                if ((new_col >= 0 && new_col < W) && (new_row >= 0 && new_row < H) && (grid[new_row][new_col]%2 == 1)) {
                    alive_neighbors++;
                }
            }
            if (grid[row][col]) {
                if (alive_neighbors<2 || alive_neighbors>3) {
                    grid[row][col] = 1;
                } else {
                    grid[row][col] = 3;
                }
            } else {
                if (alive_neighbors == 3) {
                    grid[row][col] = 2;
                } else {
                    grid[row][col] = 0;
                }
            }
        }
    }
}

void recalc_grid(std::vector<std::vector<int>>& grid) {
    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell <= 1) {
                cell = 0;
            } else {
                cell =1;
            }
        }
    }
}

int main(){
    std::vector<std::vector<int>> grid = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}};
    next_state_new_grid(grid);

    for (const auto& r : grid) {
        for (const auto& cell: r) {
            std::cout<<cell<< " ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    
    grid = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}};
    next_state_same_grid(grid);
    recalc_grid(grid);

    for (const auto& r : grid) {
        for (const auto& cell: r) {
            std::cout<<cell<< " ";
        }
        std::cout<<std::endl;
    }
    return 0;
}

#if 0

class GameOfLife:
    
    def __init__(self, board):
        self.board = board
        self.R = len(board) #Corner case: null board or the board is [] or [[]]
        self.C = len(board[0])

        //2 = currently dead, will be alive
        //3 = currently alive, will be dead
        self.neighbors = ((1, 0), (1, -1), (0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1))

    def convertValues(self):
        for rowIdx, row in enumerate(self.board):
            for colIdx, value in enumerate(row):
                if value == 2:
                    self.board[rowIdx][colIdx] = 1
                elif value == 3:
                    self.board[rowIdx][colIdx] = 0

    def runIteration(self):
        for rowIdx, row in enumerate(self.board):
            for colIdx, value in enumerate(row):
                numLivingNeighbors = 0
                for dy, dx in self.neighbors:
                    if (0 <= rowIdx + dy < self.R) and (0 <= colIdx + dx < self.C) and self.board[rowIdx + dy][colIdx + dx] in (1, 3):
                        numLivingNeighbors += 1
                if value == 1 and (numLivingNeighbors > 3 or numLivingNeighbors < 2):
                    self.board[rowIdx][colIdx] = 3
                elif value == 0 and (numLivingNeighbors == 3):
                    self.board[rowIdx][colIdx] = 2
        print(self.board)

if __name__ == "__main__":
    game = GameOfLife(board)
    game.runIteration()
    game.convertValues()
    print(game.board)

#endif

/*
Infinite:
def gameOfLifeInfinite(self, live):
    ctr = collections.Counter((I, J)
                              for i, j in live
                              for I in range(i-1, i+2)
                              for J in range(j-1, j+2)
                              if I != i or J != j)
    return {ij for ij in ctr if ctr[ij] == 3 or ctr[ij] == 2 and ij in live}

def gameOfLife(self, board):
    live = {(i, j) for i, row in enumerate(board) for j, live in enumerate(row) if live}
    live = self.gameOfLifeInfinite(live)
    for i, row in enumerate(board):
        for j in range(len(row)):
            row[j] = int((i, j) in live)
*/
