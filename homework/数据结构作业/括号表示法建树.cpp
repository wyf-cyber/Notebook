#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

struct TreeNode {
    // 定义二叉链式存储结构
    TreeNode* left;
    TreeNode* right;
    int val;
};

void findNum(const string& s, int& index, string& elm) {
    // 找出从index开始，长度为len的数字
    int ans = 0;
    int len = 0;
    bool isNum = false;
    while (index < s.length()) {
        if (s[index] == ',' || s[index] == '(' || s[index] == ')') {
            break;
        }
        int temp = s[index] - '0';
        ans = ans * 10 + temp;
        index++;
        len++;
        isNum = true;
    }
    if (isNum) {
        elm = to_string(ans);
    }
    else if (index < s.length() && len == 0) {
        // 将符号填入结果
        elm = string(1, s[index]);
        index++;
    }
    else {
        elm = "";
    }
}

vector<string> trans(string s) {
    // 将输入的字符串切割成数字、括号和逗号
    vector<string> nums;
    for (int i = 0; i < s.length();) {
        string elm = "";
        findNum(s, i, elm);
        if (!elm.empty()) nums.push_back(elm);
    }
    return nums;
}

TreeNode* build(vector<string>& s) {
    stack<TreeNode*> st;
    TreeNode* p;
    TreeNode* root = nullptr; // Initialize root to nullptr
    bool flag = false;
    int i = 0;

    while (i < s.size()) {
        if (s[i] == "(") {
            st.push(p);
            flag = true;
        } else if (s[i] == ")") {
            st.pop();
        } else if (s[i] == ",") {
            flag = false;
        } else {
            p = new TreeNode();
            p->val = stoi(s[i]);
            if (root == nullptr) {
                root = p;
            } else {
                if (flag && !st.empty()) {
                    st.top()->left = p;
                } else if (!st.empty()) {
                    st.top()->right = p;
                }
            }
        }
        i++;
    }
    return root;
}
void findLeaf(TreeNode* root, vector<int>& leaf) {
    // 前序遍历搜索所有叶子结点
    if(root == nullptr) return;
    else if(root->left == nullptr && root->right == nullptr) {
        // 发现一个叶子结点
        leaf.push_back(root->val);
        return;
    }
    if(root->left) findLeaf(root->left, leaf);
    if(root->right) findLeaf(root->right, leaf);
}

void levelOrderTraversal(TreeNode* root) {
    queue<TreeNode*> q;
    q.push(root);
    while(!q.empty()) {
        int n = q.size();
        for(int i = 0; i < n; i++) {
            TreeNode* node = q.front();
            q.pop();
            if(node) cout << node->val << " ";
            if(node->right) q.push(node->right);
            if(node->left) q.push(node->left);
        }
    }
}


int main() {
    ifstream input("in.txt");
    string s;
    vector<int> leaf;
    vector<int> allNode;

    input >> s;
    vector<string> elms = trans(s);

    /*日志代码
    for(int i = 0; i < elms.size(); i++) {
        cout << elms[i] << " ";
    }
    cout << endl;
    */
    TreeNode* root = build(elms);
    // cout << "build() runs successfully!" << endl;
    findLeaf(root, leaf);
    // cout << "findLeaf() runs successfully!" << endl;
    int n = leaf.size();
    // 正序输出所有叶子结点
    for (int i = 0; i < n; i++) {
        cout << leaf[i] << " ";
    }
    cout << endl;
    // 逆序输出所有叶子节点
    for (int i = n - 1; i > -1; i--) {
        cout << leaf[i] << " ";
    }
    cout << endl;
    // 层序遍历所有节点
    levelOrderTraversal(root);
}
