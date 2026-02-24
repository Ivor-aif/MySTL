#include <vector>

#include "../headers/BiTree.h"
using namespace std;

// Welcome to submit any other usages as below!

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/binary-tree-preorder-traversal/description/
 * @date 2026-02-24, 22:36(UTC+8)
 * @consume 0ms, 11.50MB
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
vector<int> preorderTraversal(TreeNode* root) {
    using namespace mySTL::structures;
    using namespace mySTL::containers;
    auto copy = [&](auto&& self, BiTree<int>& bt, BiTree<int>::Node* root, TreeNode* sub, int branch) -> void {
        if (!sub) {
            return;
        }
        BiTree<int>::Node* obj = branch == 0 ? bt.insertLeft(root, sub->val) : bt.insertRight(root, sub->val);
        self(self, bt, obj, sub->left, 0);
        self(self, bt, obj, sub->right, 1);
    };

    BiTree<int> bt;
    if (root) {
        BiTree<int>::Node* obj = bt.insertRoot(root->val);
        copy(copy, bt, obj, root->left, 0);
        copy(copy, bt, obj, root->right, 1);
    }
    Array<int> ans = bt.preOrder();
    return vector(ans.begin(), ans.end());
}

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/binary-tree-inorder-traversal/description/
 * @date 2026-02-24, 22:38(UTC+8)
 * @consume 3ms, 11.23MB
 */
vector<int> inorderTraversal(TreeNode* root) {
    using namespace mySTL::structures;
    using namespace mySTL::containers;
    auto copy = [&](auto&& self, BiTree<int>& bt, BiTree<int>::Node* root, TreeNode* sub, int branch) -> void {
        if (!sub) {
            return;
        }
        BiTree<int>::Node* obj = branch == 0 ? bt.insertLeft(root, sub->val) : bt.insertRight(root, sub->val);
        self(self, bt, obj, sub->left, 0);
        self(self, bt, obj, sub->right, 1);
    };

    BiTree<int> bt;
    if (root) {
        BiTree<int>::Node* obj = bt.insertRoot(root->val);
        copy(copy, bt, obj, root->left, 0);
        copy(copy, bt, obj, root->right, 1);
    }
    Array<int> ans = bt.inOrder();
    return vector(ans.begin(), ans.end());
}

/**
 * @author Ivor
 * @source https://leetcode.cn/problems/binary-tree-postorder-traversal/description/
 * @date 2026-02-24, 22:39(UTC+8)
 * @consume 0ms, 11.24MB
 */
vector<int> postorderTraversal(TreeNode* root) {
    using namespace mySTL::structures;
    using namespace mySTL::containers;
    auto copy = [&](auto&& self, BiTree<int>& bt, BiTree<int>::Node* root, TreeNode* sub, int branch) -> void {
        if (!sub) {
            return;
        }
        BiTree<int>::Node* obj = branch == 0 ? bt.insertLeft(root, sub->val) : bt.insertRight(root, sub->val);
        self(self, bt, obj, sub->left, 0);
        self(self, bt, obj, sub->right, 1);
    };

    BiTree<int> bt;
    if (root) {
        BiTree<int>::Node* obj = bt.insertRoot(root->val);
        copy(copy, bt, obj, root->left, 0);
        copy(copy, bt, obj, root->right, 1);
    }
    Array<int> ans = bt.postOrder();
    return vector(ans.begin(), ans.end());
}
