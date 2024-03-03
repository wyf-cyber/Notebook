#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>

using namespace std;
struct BTNode {
    string data;
    BTNode* lchild;  
    BTNode* rchild;
    BTNode() {
        lchild = nullptr;
        rchild = nullptr;
    }
    BTNode(string s) : data(s), lchild(nullptr), rchild(nullptr) {}
};

bool isOperator(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

int getPrecedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;  // for "(" and ")"
}
// 中缀转后缀表达式
vector<string> shift(const vector<string>& infix) {
    vector<string> postfix;
    stack<string> ops;

    for (const string& token : infix) {
        if (isdigit(token[0]) || (token.length() > 1 && isdigit(token[1]))) {
            // 如果本字符串所代表的是数字，直接加入后缀表达式
            postfix.push_back(token);
        } else if (token == "(") {
            // "("直接压入符号栈
            ops.push(token);
        } else if (token == ")") {
            // 将符号栈中所有的符号弹出直到遇到左括号
            while (!ops.empty() && ops.top() != "(") {
                postfix.push_back(ops.top());
                ops.pop();
            }
            // 将左括号出栈
            ops.pop();  
        } else if (isOperator(token)) {
            // 如果该字符串所代表的是运算符
            // 在将某个运算符入栈前需要确保栈顶元素的优先级低于将要放入的符号
            while (!ops.empty() && getPrecedence(ops.top()) >= getPrecedence(token)) {
                postfix.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
    }
    // 遍历完成以后运算符栈中可能还有运算符，将其全部弹出并加入后缀表达式
    while (!ops.empty()) {
        postfix.push_back(ops.top());
        ops.pop();
    }

    return postfix;
}

// 利用后缀表达式生成表达式树
BTNode* buildTree(vector<string> postfix) {
    stack<BTNode*> elms;
    for(int i = 0; i < postfix.size(); i++) {
        if(i == 0) {
            BTNode* node = new BTNode(postfix[i]);
            elms.push(node);
            continue;
        }
        if(isOperator(postfix[i])) {
            BTNode* temp = new BTNode(postfix[i]);
            temp->rchild = elms.top();
            elms.pop();
            temp->lchild = elms.top();
            elms.pop();
            elms.push(temp);  // 将当前分支节点入栈
        } else {
            // 数字直接入栈
            BTNode* temp = new BTNode(postfix[i]);
            elms.push(temp);
        }
    }
    // 栈中剩余的一个树节点即为根节点
    BTNode* root = elms.top();
    elms.pop();
    return root;
}


void preOrderTraversal(BTNode* root) {
    if(root == nullptr) return;
    string temp = root->data;
    // 先处理双亲节点
    if(temp == "+" || temp == "-" || temp == "*" || temp == "/") {
        cout << temp;
    } else {
        // 当前节点为数字节点，需要打印#
        cout << temp << "#";
    }
    preOrderTraversal(root->lchild);
    preOrderTraversal(root->rchild);
}

void inOrderTraversal(BTNode* root) {
    if(root == nullptr) return;   
    // 先处理左子树
    inOrderTraversal(root->lchild);
    string temp = root->data;    
    // 处理双亲节点
    if(temp == "+" || temp == "-" || temp == "*" || temp == "/") {
        cout << temp;
    } else {
        // 当前节点为数字节点，需要打印#
        cout << temp << "#";
    }
    // 处理右子树
    inOrderTraversal(root->rchild);
}

void postOrderTraversal(BTNode* root) {
    if(root == nullptr) return;   
    postOrderTraversal(root->lchild);
    postOrderTraversal(root->rchild);
    string temp = root->data;
    // 先处理双亲节点
    if(temp == "+" || temp == "-" || temp == "*" || temp == "/") {
        cout << temp;
    } else {
        // 当前节点为数字节点，需要打印#
        cout << temp << "#";
    }
}

void levelOrderTraversal(BTNode* root) {
    queue<BTNode*> q;
    q.push(root);
    while(!q.empty()) {
        // 当前所在层次上节点的个数
        int len = q.size();
        for(int i = 0; i < len; i++) {
            BTNode* temp = q.front();
            q.pop();
            // 打印当前节点内容
            if(temp->data == "+" || temp->data == "-" || temp->data == "*" || temp->data == "/") {
                cout << temp->data;
            } else {
                cout << temp->data << "#";
            }
            if(temp->lchild) q.push(temp->lchild);
            if(temp->rchild) q.push(temp->rchild);
        }
    }
}
// 计算表达式树所存储的算式的运行结果
int calculating(BTNode* root) {
    if(root->data == "+") 
        return calculating(root->lchild) + calculating(root->rchild);
    else if(root->data == "-") 
        return calculating(root->lchild) - calculating(root->rchild);
    else if(root->data == "*") 
        return calculating(root->lchild) * calculating(root->rchild);
    else if(root->data == "/")
        return calculating(root->lchild) / calculating(root->rchild);
    else {
        // 当前节点是数字节点
        return stoi(root->data);
    } 
}

int main() {
    char c;
    int count = -1;                 // 记录当前数组的最后一个元素的位置
    bool preIsNum = false;          // 记录上一个字符是否是数字
    vector<string> infix;
    string input; // 用于读取输入的完整表达式

    // 读取整个表达式
    getline(cin, input);

    for (char c : input) {
        if (c == ' ') {
            preIsNum = false;
        } else if (c == '=') {
            // 停止读取，开始运算
            break;
        } else if (isdigit(c)) {
            if (preIsNum) {
                // 是多位数字
                infix[count] = to_string(stoi(infix[count]) * 10 + (c - '0'));
            } else {
                // 是一个新的数
                preIsNum = true;
                count++;
                infix.push_back(string(1, c));
            }
        } else {
            // 读取到运算符或括号
            preIsNum = false;
            infix.push_back(string(1, c));
            count++;
        }
    }

    BTNode* root = buildTree(shift(infix));   
    preOrderTraversal(root);
    cout << endl;
    inOrderTraversal(root);
    cout << endl;
    postOrderTraversal(root);
    cout << endl;
    levelOrderTraversal(root);
    cout << endl;
    cout << calculating(root);
}