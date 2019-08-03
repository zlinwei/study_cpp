//
// Created by linwei on 2019-08-03.
//
#include <iostream>
#include <boost/assert.hpp>
#include <cmath>

class Solution {
public:
    /**
     * @param a: A 32bit integer
     * @param b: A 32bit integer
     * @param n: A 32bit integer
     * @return: An integer
     */
    int fastPower(int a, int b, int n) {
        // write your code here
        if (n == 0)return 1 % b;
        if (n == 1)return a % b;

        int temp = fastPower(a, b, n / 2);
        if (n % 2 == 0) {
            return (temp * temp) % b;
        } else {
            return ((temp * temp) % b * a) % b;
        }

    }

    /**
     * pow(a,n)
     * @param a
     * @param n
     * @return
     */
    int
    fastPower(int a, int n) {
        if (n == 0)return 1;
        if (n == 1)return a;

        int temp = fastPower(a, n / 2);
        if (n % 2 == 0) {
            return temp * temp;
        } else {
            return temp * temp * a;
        }
    }
};

int main() {
    Solution solution{};
    uint64_t a = 2, n = 22;
    uint64_t b = 17;
    std::cout << (uint64_t) std::pow(a, n) << " " << solution.fastPower(a, n) << std::endl;
    std::cout << ((uint64_t) std::pow(a, n) % b) << " " << solution.fastPower(a, b, n) << std::endl;
    return 0;
}