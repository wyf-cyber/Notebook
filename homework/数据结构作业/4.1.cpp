#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;

bool isConsequence(int n, vector<char> inStk, vector<char> outStk) {
    stack<char> s;
    int inIndex = 0;
    int outIndex = 0;

    while (outIndex < n) {
        // 如果栈为空或者栈顶元素不等于出栈序列中的当前元素，说明栈顶元素不在这一时刻出栈
        // 将进栈序列中的元素入栈直到找到将要出栈的元素，将其压入栈顶
        while (inIndex < n && (s.empty() || s.top() != outStk[outIndex])) {
            s.push(inStk[inIndex]);
            inIndex++;
        }

        // 如果栈顶元素等于出栈序列中的当前元素，将其出栈
        if (!s.empty() && s.top() == outStk[outIndex]) {
            s.pop();
            outIndex++;
        } else {
            // 栈为空或将入栈序列剩余的所有元素加入后栈顶依然无法匹配出栈序列中的当前元素，说明一定不是合适的出栈序列
            // 注意即使此时栈为空，但出栈序列仍未被完全遍历，说明不匹配
            return false;
        }
    }

    // 此时出栈序列遍历完毕，如果栈为空，说明是合适的出栈序列
    return s.empty();
}


int main() {
    int n;
    cin >> n;
    vector<char> instk(n, ' ');
    vector<char> outstk(n, ' ');
    for(int i = 0; i < n; i++) {
        cin >> instk[i];
    }
    for(int i = 0; i < n; i++) {
        cin >> outstk[i];
    }
    // 注意如果使用cout直接输出bool型变量，输出将自动转换成0或1，不符合要求 
    bool result = isConsequence(n, instk, outstk);
    if(result == true) cout << "true";
    else cout << "false";
    return 0;
}