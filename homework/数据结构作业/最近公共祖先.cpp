// POJ1330—求树中两个结点的最近公共祖先（LCA）
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int checkLevel(int index, const vector<int>& parent) {
    int ans = 0;
    while(index != -1) {
        index = parent[index - 1];
        ans++;
    }
    return ans;
}

void changeLevel(int& index, int pace, const vector<int>& parent) {
    for(int i = 0; i < pace; i++) {
        index = parent[index - 1];
    }
}

int find(int p, int q, const vector<int>& parent) {
    while(p != q && p != -1) {
        p = parent[p - 1];
        q = parent[q - 1];
    }
    return p;
}

int main() {
    ifstream input("in.txt");
    int ans;
    int n;
    input >> n;
    // 创建一个数组存储所有节点的双亲节点
    int root;
    vector<int> parent(n, -1);
    for(int i = 0; i < n - 1; i++) {
        int p, c;
        input >> p >> c;
        parent[c - 1] = p;
    }
    // 找出头结点
    for(int i = 0; i < n; i++) {
        if(parent[i] == -1) root = i + 1;
    }

    int p, q;
    input >> p >> q;
    int pLevel = checkLevel(p, parent);
    int qLevel = checkLevel(q, parent);
    // 先将两个节点放在同一层
    if(pLevel > qLevel) {
        changeLevel(p, pLevel - qLevel, parent);
    } else if(pLevel < qLevel) {
        changeLevel(q, qLevel - pLevel, parent);
    }
    // 两个节点指针同时向上遍历，当两个指针第一次指向同一个节点的时候说明找到目标节点
    ans = find(p, q, parent);
    if(ans == -1) ans = root;
    cout << ans;
}