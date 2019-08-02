//
// Created by Administrator on 2019/8/2.
//
#include <deque>
#include <vector>
#include <iostream>

using namespace std;

class MonotonicQueue {
public:
    void push(int val) {
        while (!dq.empty() && dq.back() < val) {
            dq.pop_back();
        }
        dq.push_back(val);
    }

    void pop() {
        dq.pop_front();
    }

    int max() {
        return dq.front();
    }

private:
    deque<int> dq;
};

class Solution {
public:
    /**
     * @param nums: A list of integers.
     * @param k: An integer
     * @return: The maximum number inside the window at each moving.
     */
    vector<int> maxSlidingWindow(vector<int> &nums, int k) {
        // write your code here
        if (k <= 0 || k > nums.size()) {
            return vector<int>();
        }

        if (k == 1) {
            return nums;
        }

        MonotonicQueue mq;
        vector<int> ans;
        for (int i = 0; i < nums.size(); i++) {
            mq.push(nums[i]);
            if (i >= k - 1) {
                ans.push_back(mq.max());
                if (nums[i - k + 1] == mq.max()) {
                    mq.pop();
                }
            }
        }

        return ans;
    }
};


int main(int, char *[]) {
    std::vector<int> input = {1, 2, 6, 2, 8, 12, 23, 3, 5, 12, 4, 12, 5, 6, 1, 2, 8, 2};
    Solution solution{};
    auto res = solution.maxSlidingWindow(input, 5);

    for (auto &item: res) {
        cout << item << " ";
    }
    cout << endl;
    return 0;
}