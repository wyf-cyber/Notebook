# N皇后问题
## 一、题目描述
N皇后问题是指将 n 个皇后放在大小为 n×n 的国际象棋棋盘上，使得皇后不能相互攻击到，即任意两个皇后都不能处于同一行、同一列或同一斜线上。当前若给定n的值，求所有满足条件的棋子摆法。
![Alt](https://img-blog.csdnimg.cn/d3464d32491d4be5bd727f8b73b11fd2.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAU29sa2F0dCdz,size_10,color_FFFFFF,t_70,g_se,x_16)

**输入格式：**
输入为一行包含n的值的整型常量
输入样例：
>5

**输出格式：**
- 每个解决方案占 n 行，每行输出一个长度为 n 的字符串，用来表示完整的棋盘状态。
- 其中字符 '-' 表示该位置的方格状态为空，'Q' 表示该位置的方格上有一个皇后。
- 每个方案输出完成后，输出一个空行。
- 行末不能有多余空格。
- 输出方案的顺序任意，不重复且没有遗漏即可。

输出样例：
Solution 1:
_Q__
___Q
Q___
__Q_

Solution 2:
__Q_
Q___
___Q
_Q__


**数据范围：**
1 <= n <= 9

## 深度优先搜索解法
### （一）定义状态空间
使用一个 n×n 大小的二维字符数组存储棋盘上各皇后的放置情况。设状态空间中的一点为 n×n 的二维字符数组。
### （二）定义操作规则
在棋盘上放置n个皇后的过程可以模拟为一个搜索过程:
每次只放置一个皇后，第i次在第i行放置第i个皇后，每一次放置总让它按照从左向右的顺序试探第i行中的每一个格子；
- 当遇到第一个按照规则可以放置皇后且在以前的搜索过程中没有放置过皇后的格子时，将第i个皇后放置在该格子上，并记录第i个皇后所处的列，继续开始搜索第i+1行并重复以上步骤；
- 如果遍历整行以后没有找到符合条件的格子，则进行回溯操作，将上一行放置皇后的位置标记为已搜索过，并继续从左向右搜索符合条件的格子，重复以上步骤；
- 当放置第n行皇后以后继续调用递归函数；
- 当发现将要放置第n+1个皇后时，将当前状态修改为“已找到”并根据先前记录的各个皇后的列数打印出可行解；
- 当本层函数调用下一层函数以后发现状态变为“已找到”，则停止遍历寻找，立即返回；
- 如果函数调用完毕且从没有将状态修改为“已找到”，打印"No solution found."表示没有可行解；


### （三）算法代码
~~~cpp
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;


bool isValid(vector<int> pos, int i, int newpos) {
    // newpos of the queue[i] is valid?
    bool res = true;
    for(int j = 0; j < i; j++) {
        // 判断 queue[i] 与 queue[j] 之间是否存在违规位置关系
        if(newpos == pos[j] || pos[j] + (i-j) == newpos || pos[j] - (i-j) == newpos) {
            res = false;
        }
    }
    return res;
}
void backtracking(int n, int i, vector<int> & pos, vector<vector<int>> & ans) {
    if(i >= n) return;
    for(int m = 0; m < n; m++) {
        if(!isValid(pos, i, m)) continue;
        pos[i] = m;
        if(i == n-1) {
            ans.push_back(pos);
        }
        backtracking(n, i+1, pos, ans);
        pos[i] = -1;
    }
}
void printAns(vector<vector<int>> ans) {
    if(ans.empty()) {
        // 未发现可行解
        cout << "No solution for this size." << endl;
        return;
    }
    for(int i = 0; i < ans.size(); i++) {
        // 打印ans[i]个解
        cout << "Solution " << i+1 << ":" << endl;
        for(int j = 0; j < ans[i].size(); j++) {
            // 遍历每一行
            string line(ans[i].size(), '#');
            line[ans[i][j]] = 'Q';
            cout << line << endl;
        }
        cout << endl;
    }
}
int main() {
    int n = 0;
    cout << "Please enter the length(width): ";
    while(cin >> n) {
        auto start = chrono::high_resolution_clock::now();
        // 记录每个皇后的位置，数组的索引就是皇后的序号，数组元素的值就是该皇后的列数
        vector<vector<int>> ans;
        vector<int> pos(n, -1);
        backtracking(n, 0, pos, ans);       
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "Time taken by function: " << duration.count() << endl;
        
        printAns(ans);
 
        cout << "Please enter the length(width): ";
    }
}
~~~


