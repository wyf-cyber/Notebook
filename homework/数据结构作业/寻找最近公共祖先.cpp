#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct TreeNode {
    int index;       // 当前节点的编号
    int tempNum = 0;     // 当前节点的孩子节点的个数
    vector<TreeNode*> children;    // 存储孩子节点
    TreeNode(int i) : index(i) {}
};

// 后序遍历找出最大深度的公共节点
TreeNode* find(TreeNode* p, TreeNode* q, TreeNode* root) {
    if(root == p || root == q || root == NULL) {
        return root;
    }
    // 先处理子节点
    TreeNode* targetOne = NULL;
    TreeNode* targetTwo = NULL;
    for(int i = 0; i < root->tempNum; i++) {
        if(find(p, q, root->children[i]) == p) targetOne = p;
        else if(find(p, q, root->children[i]) == q) targetTwo = q;
        // 如果已经找到两个节点，直接返回当前节点
        if(targetOne && targetTwo) return root;      
    }
    if(targetOne) return targetOne;
    else return targetTwo;
}

int main() {
    ifstream input("in.txt");
    int n;
    input >> n;
    // 初始化树
    TreeNode* root;
    vector<TreeNode*> tree;
    vector<bool> isRoot(n, true);
    for(int i = 0; i < n; i++) {
        // 注意节点序号从1开始
        TreeNode* node = new TreeNode(i + 1); 
        tree.push_back(node);
    } 
    // 读取输入数据构建节点间的关系
    for(int i = 0; i < n - 1; i++) {
        int parent = 1;
        int child = 2;
        input >> parent >> child;
        isRoot[child - 1] = false;
        tree[parent - 1]->children.push_back(tree[child - 1]);
        tree[parent - 1]->tempNum++;
    }
    // 找出根节点
    for(int i = 0; i < n; i++) {
        if(isRoot[i]) root = tree[i];
    }
    // 接收所要操作的两个节点
    int p, q;
    input >> p >> q;
    TreeNode* answer = find(tree[p - 1], tree[q - 1], root);
    if(answer) cout << answer->index; 
}