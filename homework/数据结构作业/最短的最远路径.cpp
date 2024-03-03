#include <iostream>
#include <vector>
#define INT_MAX 0x7fffffff
using namespace std;

int dijkstra(const vector<vector<int>>& a, int m, int n) {
    // 找出从点m到点n的最短路径
    vector<int> dist(a.size(), INT_MAX);
    vector<bool> inS(a.size(), false);
    // 将点m加入集合S中
    dist[m] = 0;
    while(!inS[n]) {
        int minDist = INT_MAX;
        int nextNode = -1;
        for(int i = 0; i < dist.size(); i++) {
            if(inS[i]) continue;   // 跳过已经存入S集合中的节点
            if(minDist > dist[i]) {
                minDist = dist[i];
                nextNode = i;
            }
        }
        // 如果没有从m到n的路径，跳出循环
        if(nextNode == -1) break;
        // 将距离S集合最近的节点加入S集合中
        inS[nextNode] = true;
        // 更新dist数组
        for(int j = 0; j < dist.size(); j++) {
            if(inS[j] || a[nextNode][j] == 0) continue;
            else if(dist[nextNode] + a[nextNode][j] < dist[j]) {
                dist[j] = dist[nextNode] + a[nextNode][j];
            }
        }
    }
    return dist[n];
}

// 找出指定节点到其他节点的最远路径长度
// 如果没有路径，返回-1
int fartherest(const vector<vector<int>>& a, int n) {
    int ans = -1;
    for(int i = 0; i < a.size(); i++) {
        if(i == n) continue;
        int temp = dijkstra(a,n,i);
        if(ans < temp && temp != INT_MAX) ans = temp;
    }
    return ans;
}

int find(const vector<vector<int>>& a) {
    // 找出拥有最短最远路径的节点
    // 挑选一个节点作为根节点
    int ans = INT_MAX;
    int node = -1;
    for(int i = 0; i < a.size(); i++) {
        int temp = fartherest(a, i);
        if(temp != -1 && ans > temp) {
            ans = temp;
            node = i;
        }
    }
    return node + 1;
}

int main() {
    int n;
    cin >> n;
    // 初始化矩阵
    vector<vector<int>> a(n, vector<int>(n, 0));
    // 读取矩阵的值
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }
    // 计算结果
    cout << find(a);
}