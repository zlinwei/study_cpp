//
// Created by linwei on 2019-07-29.
//

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class TreeNode {
public:
    int val;
    TreeNode *left, *right;

    explicit TreeNode(int val) {
        this->val = val;
        this->left = this->right = NULL;
    }
};


class Solution {
public:
    /**
     * @param root: A Tree
     * @return: Level order a list of lists of integer
     */
    vector<vector<int>> levelOrder(TreeNode *root) {
        // write your code here
        vector<vector<int>> output;
        queue<TreeNode *> q;
        if (root)
            q.push(root);

        int i = 1, j = 0;
        int level = 1;
        vector<int> current;
        while (!q.empty()) {
            auto tmp = q.front();
            current.push_back(tmp->val);
            if (tmp->left) {
                q.push(tmp->left);
                j++;
            }
            if (tmp->right) {
                q.push(tmp->right);
                j++;
            }
            i--;
            q.pop();
            if (i == 0) {
                if (level % 2 == 0)
                    reverse(current.begin(), current.end());
                output.push_back(current);
                current.clear();
                level++;
                i = j;
                j = 0;
            }

        }


        return output;
    }

};

int main(int, char *[]) {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);

    Solution solution{};
    auto result = solution.levelOrder(root);
    for (auto item : result) {
        std::cout << "[";
        for (size_t i = 0; i < item.size(); ++i) {
            std::cout << item[i] << ",";
        }
        std::cout << "]," << std::endl;
    }
    std::cout << endl;
}