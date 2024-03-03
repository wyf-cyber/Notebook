#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef struct BTNode {
    string data;
    BTNode* lchild;
    BTNode* rchild;
    BTNode() : data(""), lchild(nullptr), rchild(nullptr) {}
    BTNode(string d) : data(d), lchild(nullptr), rchild(nullptr) {}
} TreeNode;

// 根据某个0-1序列构建哈夫曼树的相应路径并将数据存入叶子结点
void constructing(BTNode*& root, string code, string value) {
    BTNode* cur = root;
    for (char c : code) {
        switch (c) {
        case '0':
            // 向左
            if (cur->lchild) cur = cur->lchild;
            else {
                BTNode* node = new BTNode();
                cur->lchild = node;
                cur = cur->lchild;
            }
            break;
        case '1':
            // 向右分支
            if (cur->rchild) cur = cur->rchild;
            else {
                BTNode* node = new BTNode();
                cur->rchild = node;
                cur = cur->rchild;
            }
            break;
        default:
            cur = cur;
        }
    }
    // 将所给值记录到叶子节点
    cur->data = value;
}



string decoding(string& code, BTNode* root) {
    string ans;
    BTNode* cur = root; // Initialize cur to root
    for (char c : code) {
        switch (c) {
        case '0':
            if (cur->lchild) {
                cur = cur->lchild;
                if(cur->data.size() > 0) {
                    // 第一次找到相应的数值
                    ans += cur->data;
                    cur = root;
                }
            } else {
                // 找到叶子节点
                ans += cur->data;
                cur = root; // Reset cur to root
            }
            break;
        case '1':
            if (cur->rchild) {
                cur = cur->rchild;
                if(cur->data.size() > 0) {
                    // 第一次找到相应数值
                    ans += cur->data;
                    cur = root;
                }
            } else {
                // 找到叶子结点
                ans += cur->data;
                cur = root; // Reset cur to root
            }
            break;
        default:
            cur = cur;
        }
    }
    if(cur->data.size() > 0) ans += cur->data;
    return ans;
}

int main() {
    ifstream input("in.txt");
    int n;
    BTNode* root = new BTNode(); // Initialize root
    // 构建哈夫曼树
    cin >> n;
    cin.ignore(); // 跳过读取n的那一行，从下一行开始读取
    for (int i = 0; i < n; i++) {
        string rule;
        getline(cin, rule);
        size_t div = rule.find(':');
        string value = rule.substr(0, div);   // [0, div - 1]
        string code = rule.substr(div + 1, rule.length() - div - 1);  // [div + 1, rule.length() - div - 1]
        constructing(root, code, value);
    }
    // 译码部分
    // 将文件流 input 中的内容读取到 fileContent 字符串中。fileContent 就包含了整个文件的内容，是一个字符串对象
    string fileContent((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    
    cout << decoding(fileContent, root);

    // 释放内存
    delete root;

    return 0;
}
