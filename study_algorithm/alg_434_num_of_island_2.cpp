//
// Created by linwei on 2019-08-20.
//

#include<iostream>
#include<vector>

using namespace std;

/*
* 岛屿的个数II
*
* 给定 n，m，分别代表一个2D矩阵的行数和列数，同时，给定一个大小为 k
 * 的二元数组A。起初，2D矩阵的行数和列数均为 0，即该矩阵中只有海洋。
 * 二元数组有 k 个运算符，每个运算符有 2 个整数 A[i].x, A[i].y，你
 * 可通过改变矩阵网格中的A[i].x]，[A[i].y] 来将其由海洋改为岛屿。请
 * 在每次运算后，返回矩阵中岛屿的数量。
* 注意事项
* 0 代表海，1 代表岛。如果两个1相邻，那么这两个1属于同一个岛。我们只考虑上下左右为相邻。
* 样例
* 给定 n = 3, m = 3， 二元数组 A = [(0,0),(0,1),(2,2),(2,1)].
* 返回 [1,1,2,2].
*/
struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}

    Point(int a, int b) : x(a), y(b) {}
};

class Solution {
public:
    /**
    * @param n: An integer
    * @param m: An integer
    * @param operators: an array of point
    * @return: an integer array
    */
    vector<int> numIslands2(int n, int m, vector<Point> &operators) {
        // write your code here
        vector<int> res;
        if (n <= 0 || m <= 0 || operators.empty())
            return res;
        vector<int> nums(n * m + 1, 0);//下标表示岛屿位置
        vector<vector<int>> dic{{1,  0},
                                {-1, 0},
                                {0,  1},
                                {0,  -1}};//方便遍历上下左右四个位置
        int count = 0;
        for (auto c : operators) {
            int pos = c.x * m + c.y + 1;
            //若nums[pos]不为0,则岛屿已经出现过,直接返回现有的岛屿数目
            if (nums[pos] > 0) {
                res.push_back(count);
                continue;
            }
            //初始化nums[pos]为pos,表示pos最高层的岛屿是本身,此时岛屿数+1
            nums[pos] = pos;
            count++;
            for (auto t : dic) {
                int a = c.x + t[0];
                int b = c.y + t[1];
                int tmp = a * m + b + 1;
                if (a < 0 || a >= n || b < 0 || b >= m || nums[tmp] == 0)
                    continue;
                //tmp表示pos的相邻位置,root为tmp的最高层岛屿
                //root!=pos表示pos与root并不属于同一个大岛屿,把pos并入root所在的大岛,
                //nums[pos]=root,并将pos置为root避免重复计算,此时岛屿数目-1
                int root = unionSearch(tmp, nums);
                if (root != pos) {
                    nums[pos] = root;
                    pos = root;
                    --count;
                }
            }
            res.push_back(count);
        }
        return res;
    }

    //寻找root的最高层岛屿
    int unionSearch(int root, vector<int> &nums) {
        while (root != nums[root]) {
            nums[root] = nums[nums[root]];
            root = nums[root];
        }
        return root;
    }
};


int main(int,char*[]){

    return 0;
}