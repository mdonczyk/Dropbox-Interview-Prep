/*
Dropbox

Number of Islands


Given a 2d grid map of 1s (land) and 0s (water), count the number of islands.
An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically.
You may assume all four edges of the grid are all surrounded by water.

i.e. count the number of connected components in a grid.
Ask if you can modify the grid. If you can, then you can use a flood fill.
If not, then you'll need to use O(Rows * Cols) space to store a grid or set to store the visited spots.
*/

#include <vector>
#include <iostream>
#include <queue>
#include <deque>

void bfs(std::vector<std::vector<char>>& grid, int row, int col) {
    std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int W = grid[0].size();
    int H = grid.size();
    //BFS
    //for bfs queue is used
    std::queue<std::pair<int, int>> q;
    q.push({row, col});
    // visit cell or flood idc
    grid[row][col] = '0';
    while (!q.empty()) {
        std::pair<int, int> p = q.front(); // for BFS the oldest element is investigated
        q.pop();
        for (const auto& dir : directions) {
            int new_row = p.first + dir.first;
            int new_col = p.second + dir.second;
            //check if it fits the grid and if its a land
            if ((new_row >= 0 && new_row <  H) && (new_col >= 0 && new_col < W) && (grid[new_row][new_col] == '1')) {
                q.push({new_row, new_col});
                grid[new_row][new_col] = '0';
            }
        }
    }
}

void dfs(std::vector<std::vector<char>>& grid, int row, int col) {
    std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int W = grid[0].size();
    int H = grid.size();
    //DFS
    //for dfs deque is used
    std::deque<std::pair<int, int>> dq;
    dq.push_back({row, col});
    // visit cell or flood idc
    grid[row][col] = '0';
    while (!dq.empty()) {
        std::pair<int, int> p = dq.back(); // for DFS the most recent element is investigated
        dq.pop_back();
        for (const auto& dir : directions) {
            int new_row = p.first + dir.first;
            int new_col = p.second + dir.second;
            //check if it fits the grid and if its a land
            if ((new_row >= 0 && new_row <  H) && (new_col >= 0 && new_col < W) && (grid[new_row][new_col] == '1')) {
                dq.push_back({new_row, new_col});
                grid[new_row][new_col] = '0';
            }
        }
    }
}

int numIslands(std::vector<std::vector<char>> grid, const std::string& search) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    int W = grid[0].size();
    int H = grid.size();
    int islands = 0;
    // flood visited cells;
    for (int row = 0; row<H; row++) {
        for (int col = 0; col<W; col++) {
            if (grid[row][col] == '1') { // mozna wtedy tez && not in visited ale bede zalewal komórki wiec jebac
                if (search == "BFS") {
                    bfs(grid, row, col);
                } else if (search == "DFS") {
                    dfs(grid, row, col);
                }
                islands ++;
            }
        }
    }
    return islands;
}

#if 0

int numIslands(std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    int W = grid[0].size();
    int H = grid.size();
    std::vector<std::pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int islands = 0;
    // flood visited cells;
    for (int row = 0; row<H; row++) {
        for (int col = 0; col<W; col++) {
            if (grid[row][col] == '1') { // mozna wtedy tez && not in visited ale bede zalewal komórki wiec jebac
                //BFS
                //bfs(row, col):
                //for bfs queue is used
                std::queue<std::pair<int, int>> q;
                q.push({row, col});
                // visit cell or flood idc
                grid[row][col] = '0';
                while (!q.empty()) {
                    std::pair<int, int> p = q.front(); // to change to DFS you would need to get the most recet element adde and not do the first one
                    q.pop();
                    for (const auto& dir : directions) {
                        int new_row = p.first + dir.first;
                        int new_col = p.second + dir.second;
                        //check if it fits the grid and if its a land
                        if ((new_row >= 0 && new_row <  H) && (new_col >= 0 && new_col < W) && (grid[new_row][new_col] == '1')) {
                            q.push({new_row, new_col});
                            grid[new_row][new_col] = '0';
                        }
                    }
                }
                islands ++;
            }
        }
    }
    return islands;
}

#endif

int main() {

    std::vector<std::vector<char>> grid = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}};
    
    for (const auto& r : grid) {
        for (const auto& cell: r) {
            std::cout<<cell<< " ";
        }
        std::cout<<std::endl;
    }

    std::cout<<"Number of islands (BFS): " << numIslands(grid, "BFS")<<std::endl;

    std::cout<<"Number of islands (DFS): " << numIslands(grid, "DFS")<<std::endl;
    return 0;
}


#if 0
class NumberOfIslands:

    def __init__(self, grid):
        self.grid = grid
        self.R = 0
        self.C = 0
        self.num_islands = 0
        self.directions = ((-1, 0), (1, 0), (0, -1), (0, 1))
    
    def process_grid(self):
        if not self.grid or not self.grid[0]: #You got null, [], or [[]]
            return self.num_islands
        self.R = len(self.grid)
        self.C = len(grid[0])
        for row in range(self.R):
            for col in range(self.C):
                if self.grid[row][col] == 1: #If you find land, flood that island so that
                    //you don't double count the same island
                    self.flood(row, col)
                    self.num_islands += 1
        return self.num_islands

    def flood(self, row, col):
        if not (0 <= row < self.R) or not (0 <= col < self.C) or self.grid[row][col] != 1:
            return
        self.grid[row][col] = 0 //flood with water so that you don't revisit the square
        for dr, dc in self.directions:
            self.flood(row + dr, col + dc)

if __name__ == "__main__":
    board = [
        [0, 1, 1, 0, 0, 0],
        [1, 0, 1, 1, 0, 0],
        [1, 0, 1, 0, 1, 0]
    ]
    processor = NumberOfIslands(board)
    assert processor.process_grid() == 3

#endif

/*
FOLLOW UP:
What if there is too much data to store in memory:
Answer: Read it in row by row in the form of streams. Use a union find.
Also observe that you only need information about the previous row for the union find if reading row-by-row.
*/