//
// Created by linwei on 2019-07-24.
//
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;


int compare(const void *a, const void *b) {
    return *(int *) a > *(int *) b;
}

class Solution2 {
public:
    void sortIntegers2(vector<int> &A) {
        // write your code here
        qsort((void *) A.data(), A.size(), sizeof(int), compare);
    }

};

void showArr(int arr[], int len) {
    for (int j = 0; j < len; ++j) {
        cout << j << " ";
    }
    cout << endl;
    for (int i = 0; i < len; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << endl;
}


class Solution {
public:
    /**
     * @param A: an integer array
     * @return: nothing
     */
    void sortIntegers2(vector<int> &A) {
        // write your code here
        QSort(A.data(), 0, A.size() - 1);
    }

    void QSort(int arr[], int low, int high) {
        if (high <= low) return;
        int i = low;
        int j = high + 1;
        int key = arr[low];
        while (true) {
            while (arr[++i] < key) {
                if (i == high) {
                    break;
                }
            }

            while (arr[--j] > key) {
                if (j == low) {
                    break;
                }
            }
            if (i >= j) break;
            swap(arr[i], arr[j]);
        }

        swap(arr[low], arr[j]);
        QSort(arr, low, j - 1);
        QSort(arr, j + 1, high);

    }
};

int main(int, char *[]) {
    Solution solution{};
    vector<int> a = {3, 2, 1, 4, 5, 2};
    solution.sortIntegers2(a);
    cout << "output: \n";
    for (auto &item: a) {
        cout << item << " ";
    }
    cout << endl;
    return 0;
}