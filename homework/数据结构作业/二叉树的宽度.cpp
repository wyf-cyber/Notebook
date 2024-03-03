// 二叉树的宽度指的是具有节点数目最多的那一层的节点个数。
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef struct node {
    int val;
    struct node* lchild;
    struct node* rchild;
}BTNode;

vector<string> divide(string s) {
    // 将有效部分按照','分割并存入数组中返回
    vector<string> ans;
    string temp = "";
    for(char c : s) {
        if(c == ',') {
            ans.push_back(temp);
            temp = "";
            continue;
        }
        temp.append(1, c);
    }
    ans.push_back(temp);
    return ans;
}

BTNode* buildTree(vector<string>& arr, int index) {
    if(index >= arr.size() || arr[index] == "null") {
        return nullptr;
    }
    BTNode* node = new BTNode();
    node->val = stoi(arr[index]);
    node->lchild = buildTree(arr, index*2 + 1);
    node->rchild = buildTree(arr, index*2 + 2);
    return node;
}

int getWidth(BTNode* root, int level, int& maxWidth, vector<int>& levelWidth) {
    if (root == nullptr) {
        return 0;
    }

    if (level >= levelWidth.size()) {
        levelWidth.push_back(1);
    }
    else {
        levelWidth[level]++;
    }

    maxWidth = max(maxWidth, levelWidth[level]);

    getWidth(root->lchild, level + 1, maxWidth, levelWidth);
    getWidth(root->rchild, level + 1, maxWidth, levelWidth);

    return maxWidth;
}

int main() {
    string input;
    getline(cin, input);
    // 从整行输入中取出数据部分
    // 找出数据部分的边界
    size_t start = input.find('[');
    size_t end = input.find(']');
    string validPart = input.substr(start + 1, end - start - 1);
    
    vector<string> arr;
    // 将输入的各个节点的数值分割成字符串存入数组中
    arr = divide(validPart);

    BTNode* root = buildTree(arr, 0);

    int maxWidth = 0;
    vector<int> levelWidth;
    int width = getWidth(root, 0, maxWidth, levelWidth);

    cout <<  width << endl;

    return 0;
}
