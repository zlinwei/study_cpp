/**
* 本参考程序来自九章算法，由 @九章算法 提供。版权所有，转发请注明出处。
* - 九章算法致力于帮助更多中国人找到好的工作，教师团队均来自硅谷和国内的一线大公司在职工程师。
* - 现有的面试培训课程包括：九章算法班，系统设计班，算法强化班，Java入门与基础算法班，Android 项目实战班，
* - Big Data 项目实战班，算法面试高频题班, 动态规划专题班
* - 更多详情请见官方网站：http://www.jiuzhang.com/?source=code
*/

/**
 * Definition of TreeNode:
 * class TreeNode {
 * public:
 *     int val;
 *     TreeNode *left, *right;
 *     TreeNode(int val) {
 *         this->val = val;
 *         this->left = this->right = NULL;
 *     }
 * }
 */

#include <iostream>
#include <vector>

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

class ResultType {
public:
    bool a_exist, b_exist;
    TreeNode *node;

    ResultType(bool a, bool b, TreeNode *n) {
        a_exist = a;
        b_exist = b;
        node = n;
    }
};

class Solution {
public:
    /**
     * @param root: The root of the binary tree.
     * @param A and B: two nodes
     * @return: Return the LCA of the two nodes.
     */
    TreeNode *lowestCommonAncestor3(TreeNode *root, TreeNode *A, TreeNode *B) {
        // write your code here
        ResultType rt = helper(root, A, B);
        if (rt.a_exist && rt.b_exist)
            return rt.node;
        else
            return nullptr;
    }

    static ResultType helper(TreeNode *root, TreeNode *A, TreeNode *B) {
        if (root == nullptr)
            return {false, false, nullptr};

        ResultType left_rt = helper(root->left, A, B);
        ResultType right_rt = helper(root->right, A, B);

        bool a_exist = left_rt.a_exist || right_rt.a_exist || root == A;
        bool b_exist = left_rt.b_exist || right_rt.b_exist || root == B;

        if (root == A || root == B)
            return {a_exist, b_exist, root};

        if (left_rt.node != nullptr && right_rt.node != nullptr)
            return {a_exist, b_exist, root};
        if (left_rt.node != nullptr)
            return {a_exist, b_exist, left_rt.node};
        if (right_rt.node != nullptr)
            return {a_exist, b_exist, right_rt.node};

        return {a_exist, b_exist, nullptr};
    }
};


int main(int, char *[]) {


    return 0;
}