//
// Created by Administrator on 2019/8/9.
//
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param grid: a boolean 2D matrix
     * @return: an integer
     */
    int numIslands(vector<vector<bool>> &grid) {
        // write your code here
        int cnt = 0;
        int m = grid.size();
        if (m == 0)return 0;
        int n = grid.at(0).size();

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid.at(i).at(j)) {
                    DFSSetFalse(grid, i, j);
                    cnt++;
                }
            }
        }
        return cnt;
    }

    static void DFSSetFalse(vector<vector<bool>> &grid, int x, int y) {
        int m = grid.size();
        int n = grid.at(0).size();
        if (x < 0 || x >= m)return;
        if (y < 0 || y >= n)return;
        if (!grid[x][y])return;

        grid.at(x).at(y) = false;
        DFSSetFalse(grid, x + 1, y);
        DFSSetFalse(grid, x - 1, y);
        DFSSetFalse(grid, x, y + 1);
        DFSSetFalse(grid, x, y - 1);

    }
};


int main(int, char *[]) {
    Solution solution{};
    std::vector<vector<bool >> input = {
            {true,  true,  false, false, false},
            {false, true,  false, false, true},
            {false, false, false, true,  true},
            {false, false, false, false, false},
            {false, false, false, false, true}
    };

    auto result = solution.numIslands(input);
    std::cout << result << std::endl;

    return 0;
}