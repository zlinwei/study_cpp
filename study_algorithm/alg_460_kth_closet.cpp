//
// Created by linwei on 2019-07-27.
//

#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    /**
     * @param A: an integer array
     * @param target: An integer
     * @param k: An integer
     * @return: an integer array
     */
    vector<int> kClosestNumbers(vector<int> &A, int target, int k) {
        // write your code here
        vector<int> output;
        output.reserve(k);
        //先用二分法找最接近的第一个位置
        int low = 0, high = A.size() - 1;
        int middle = 0;
        while ((low <= high) && (low <= A.size() - 1)
               && (high <= A.size() - 1)) {

            middle = (high + low) >> 1;
            if (target == A[middle]) {
                break;
            } else if (target < A[middle]) {
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        }

        //像最接近的值两端查找
        int left, right;
        if (middle > 0) {
            left = middle - 1;
            right = middle;
        } else {
            left = middle;
            right = middle + 1;
        }


        while (k > 0) {
            //cout << left << " " << right << endl;
            if (target - A[left] <= A[right] - target && left >= 0) {
                output.push_back(A[left]);
                //cout << "left: " << left << endl;
                left--;
            } else if (right < A.size()) {
                output.push_back(A[right]);
                //cout << "right: " << right << endl;
                right++;
            }

            k--;
            //if(output.size() == k)break;
        }


        return output;
    }
};

int main() {
    Solution solution{};
    std::vector<int> A = {1, 4, 6, 10, 20};
    auto res = solution.kClosestNumbers(A, 21, 4);
    for (auto item: res) {
        std::cout << item << " ";
    }
    std::cout << endl;

    return 0;
}