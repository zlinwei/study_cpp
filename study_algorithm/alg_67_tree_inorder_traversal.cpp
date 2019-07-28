//
// Created by Administrator on 2019/7/28.
//

#include <iostream>
#include <vector>

using namespace std;

// Definition of TreeNode:
class TreeNode {
public:
    int val;
    TreeNode *left, *right;

    explicit TreeNode(int val) {
        this->val = val;
        this->left = this->right = nullptr;
    }
};


class Solution {
public:
    /**
     * @param root: A Tree
     * @return: Preorder in ArrayList which contains node values.
     */
    vector<int> inorderTraversal(TreeNode *root) {
        // write your code here
        vector<int> output;
        getNext(root,output);
        return output;
    }

    void getNext(TreeNode *root, vector<int> &output) {
        if (root == nullptr)return;
        getNext(root->left, output);
        output.push_back(root->val);
        getNext(root->right, output);
    }
};

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);

    Solution solution{};
    auto result = solution.inorderTraversal(root);
    for (auto item : result) {
        std::cout << item << " ";
    }
    std::cout << endl;

    return 0;
}