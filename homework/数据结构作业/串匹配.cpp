#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// 给定两个字符串s1和s2，求最长的s1前缀ss使得ss为s2的最长后缀，输出该字符串和其长度。
// 输入样例：
// aaariemann
// marjorieaaa
// 输出样例：
// aaa 3
void f(string s1, string s2) {
    int len = 0;
    int m = s1.length();
    int n = s2.length();
    int count = 0;
    while(count < m && count < n) {
        count++;
        string former = s1.substr(0, count);
        string latter = s2.substr(n - count, count);
        if(former == latter) {
            len = count;
        }
    }
    string ans = s1.substr(0, len);
    cout << ans << " " << len << endl;
}

int main() {
    // debug
    // string s1 = "aaariemann";
    // string s2 = "marjorieaaa";
    ifstream input("in.txt");
    string s1, s2;
    input >> s1;
    input >> s2;
    f(s1, s2);
}