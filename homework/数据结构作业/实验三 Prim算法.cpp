#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

struct ENode {
    int weight;
    int id;
    int end;
    ENode* next;
};

struct HNode {
    ENode* next;
};

vector<int> Prim(const vector<HNode*>& g, int& sum) {
    // 返回parent[]
    int n = g.size();
    // vector<bool> inS(n, false);  
    vector<int> parent(n, -1);         // 当前节点在集合S中的双亲节点序号
    vector<int> minDist(n, INT_MAX);   // 节点与集合S之间的最小距离
    // 从g[0]开始
    int cur = 1;     // 当前在S中的节点数
    minDist[0] = 0;
    // inS[0] = true;
    // 初始化
    ENode* p = g[0]->next;
    while(p) {
        minDist[p->end] = p->weight;
        parent[p->end] = 0;
        p = p->next;
    }
    while(cur < n) {
        // 找出当前距离最短的边并将其加入生成树
        cur++;
        int tempNode;
        int tempMin = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(minDist[i] < tempMin && minDist[i] > 0) {
                tempNode = i;
                tempMin = minDist[i];
            }
        }
        if(tempMin == INT_MAX) break;
        // 加入生成树并更新列表
        sum += minDist[tempNode];
        minDist[tempNode] = 0;
        //inS[tempNode] = true;
        ENode* q = g[tempNode]->next;
        while(q) {
            if(q->weight < minDist[q->end]) {
                minDist[q->end] = q->weight;
                parent[q->end] = tempNode;
            }
            q = q->next;
        }
    }
    return parent;
}

// 按照双亲存储法存储的子树信息打印边的id

void printAns(const vector<HNode*>& g, vector<int>& parent) {
    vector<int> path;
    // 遍历parent[]，找出所有包含在子树中的边
    for(int cur = 0; cur < parent.size(); cur++) {
        int pre = parent[cur];   // 找出cur节点的前驱节点
        if(pre == -1) continue;  // 跳过根节点
        // 从邻接表中找出相应的边节点并读取其id
        ENode* p = g[pre]->next;
        while(p) {
            if(p->end == cur) {
                // 找到相应边节点
                path.push_back(p->id);
                break;
            }
            p = p->next;
        }
    }
    sort(path.begin(), path.end());
    for(int i = 0; i < path.size(); i++) {
        cout << path[i] << " ";
    }
}

int main() {
    int v, e;
    cin >> v >> e;
    vector<HNode*> head;
    // 创建邻接表头
    for(int j = 0; j < v; j++) {
        HNode* node = new HNode();
        node->next = nullptr;
        head.push_back(node);
    }
    // 创建邻接表
    for(int i = 0; i < e; i++) {
        ENode* node = new ENode();
        ENode* back = new ENode();
        int begin;
        node->next = nullptr;
        back->next = nullptr;
        cin >> node->id >> begin >> node->end >> node->weight;
        back->id = node->id;
        back->end = begin;
        back->weight = node->weight;
        // 将新节点加入链表
        ENode* cur = head[begin]->next;
        if(cur == nullptr) {
            head[begin]->next = node;
        } else {
            while(cur->next) cur = cur->next;
            cur->next = node;
        }
        cur = head[node->end]->next;
        if(cur == nullptr) {
            head[node->end]->next = back;
        } else {
            while(cur->next) cur = cur->next;
            cur->next = back;
        }
    }
    int sum = 0;
    vector<int> parent = Prim(head, sum);
    cout << sum << endl;
    printAns(head, parent);
}