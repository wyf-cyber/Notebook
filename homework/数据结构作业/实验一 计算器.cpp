#include <iostream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

// 中缀表达式转后缀表达式
vector<string> shift(vector<string> infix) {
    stack<string> ops;       // 运算符栈
    vector<string> postfix;  // 后缀表达式
    for(int i = 0; i < infix.size(); i++) {
        if(infix[i] == "+" || infix[i] == "-") {
            // 如果运算符栈中为空，直接入栈
            if(ops.empty()) {
                ops.push(infix[i]);
                continue;
            }
            string s = ops.top();
            while(s == "*" || s == "/") {
                // 将高优先级的运算符出栈并加入后缀表达式中
                ops.pop();
                postfix.push_back(s);
                if(ops.empty()) {
                    // 检查是否为空，以免取空栈的栈顶
                    break;
                } 
                s = ops.top(); 
            }
            // 将当前运算符加入栈中
            ops.push(infix[i]);
        } else if(infix[i] == ")"){
            string s = ops.top();
            while(s != "(") {
                // 将左括号上面的所有运算符出栈并加入后缀表达式中
                ops.pop();
                postfix.push_back(s);
                if(ops.empty()) {
                    // 检查是否为空，以免取空栈的栈顶
                    break;
                } 
                s = ops.top(); 
            }
            // 弹出左括号
            ops.pop();
        } else if(infix[i] == "("){
            // "("直接入栈
            ops.push(infix[i]);
        } else if(infix[i] == "*" || infix[i] == "/") {
            // 将"(" 以后进栈的所有"*","/"全部出栈
            while(!ops.empty() && ops.top() != "(" && (ops.top() == "*" || ops.top() == "/")) {
                string e = ops.top();
                ops.pop();
                postfix.push_back(e);
            }
            ops.push(infix[i]);
        } else {
            // 数字直接加入后缀表达式
            postfix.push_back(infix[i]);
        }
    }
    // 如果操作完毕以后运算符栈中仍有符号，则依次弹出并添加到后缀表达式中
    while(!ops.empty()) {
        string s = ops.top();
        ops.pop();
        postfix.push_back(s);
    }
    return postfix;
}

// 计算后缀表达式
int calculating(vector<string> postfix) {
    stack<int> nums;       // 操作数栈
    for(int i = 0; i < postfix.size(); i++) {
        if(postfix[i] == "+") {
            int m = nums.top();
            nums.pop();
            int n = nums.top();
            nums.pop();
            nums.push(m + n);
        } else if(postfix[i] == "-") {
            int m = nums.top();
            nums.pop();
            int n = nums.top();
            nums.pop();
            nums.push(n - m);
        } else if(postfix[i] == "*") {
            int m = nums.top();
            nums.pop();
            int n = nums.top();
            nums.pop();
            nums.push(m * n);
        } else if(postfix[i] == "/") {
            int m = nums.top();
            nums.pop();
            int n = nums.top();
            nums.pop();
            nums.push(n / m);
        } else {
            // 读取数字
            int num = stoi(postfix[i]);
            nums.push(num);
        }
    }
    int ans = nums.top();
    nums.pop();
    return ans;
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

    cout << calculating(shift(infix));   
}