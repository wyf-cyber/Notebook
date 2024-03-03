#include <iostream>
#include <string>
#include <vector>
// #include <utility>
using namespace std;

// 获取输入字符串中逆序对的个数
int count(string s) {
    int ans = 0;
    for(int i = 0; i < s.size(); i++) {
        int temp = 0;  // 存储第i个字符与后续字符之间的逆序对的数目
        char cur = s[i];
        for(int j = i + 1; j < s.size(); j++) {
            if(cur > s[j]) temp++;
        }
        ans += temp;
    }
    return ans;
}

// 插入排序
void insertSort(vector<pair<int, string>>& content) {
    for(int i = 1; i < content.size(); i++) {
        // 将当前DNA与前面的DNA进行比较，当前面的DNA的first大于该DNA的first，则需要交换位置
        int pre = i - 1;
        int cur = i;
        while(pre >= 0) {
            if(content[pre].first > content[cur].first) {
                // 交换位置
                pair<int, string> temp = content[pre];
                content[pre] = content[cur];
                content[cur] = temp;
                cur = pre;
                pre = cur - 1;
            } else {
                break;
            }
        }
    }
} 

// 打印结果
void printAns(vector<pair<int, string>> ans) {
    for(int i = 0; i < ans.size(); i++) {
        cout << ans[i].second << endl;
    }
}

int main() {
    int len; // 字符串长度
    int n;   // 字符串的个数
    cin >> len >> n;
    cin.ignore();
    vector<pair<int, string>> content;
    for(int i = 0; i < n; i++) {
        string dna;   // 存储DNA 
        getline(cin, dna);
        pair<int, string> cur;
        cur.first = count(dna);
        cur.second = dna;
        content.push_back(cur);
    }
    // 插入排序
    insertSort(content);
    // 输出结果
    printAns(content);
}