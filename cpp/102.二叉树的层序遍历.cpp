/*
 * @lc app=leetcode.cn id=102 lang=cpp
 *
 * [102] 二叉树的层序遍历
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
#include <vector>
#include <queue>
using namespace std;
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) { return {};}

        vector<vector<int>> result;  //注意不是返回一维数组 而是每一层一起的嵌套，所以需要注意
        queue<TreeNode*> q;  // 注意这里要用指针不然对象拷贝后空指针无法处理
        q.push(root);
        while (!q.empty()) {
            int size = q.size(); // 这里是因为一层需要一起处理
            vector<int> currentLevel;
            for (int i = 0; i < size; i++) {
                TreeNode* node= q.front();
                //注意这里不用pop因为弹出不需要返回值，应该用front获取iterator后再手动处理收尾
                q.pop();

                currentLevel.push_back(node->val);
                if (node->left != nullptr) q.push(node->left);
                if (node->right != nullptr) q.push(node->right);
                
            }
            result.push_back(currentLevel);
        }
        return result;
    }
};
// @lc code=end

