//
// Created by Administrator on 2019/7/26.
//
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param n: An integer
     * @param nums: An array
     * @return: the Kth largest element
     */
    static int kthLargestElement(int n, vector<int> &nums) {
        // write your code here
    }
};


int main(int, char *[]) {
    std::vector<int> v = {
            1, 2, 8, 2, 3, 9, 1, 2, 3, 7, 5
    };

    int k = 5;
    std::cout << Solution::kthLargestElement(k,v) << std::endl;
    return 0;
}


