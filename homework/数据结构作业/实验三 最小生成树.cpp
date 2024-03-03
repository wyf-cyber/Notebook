#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct ENode {
    int begin;
    int end;
    int weight;  
    int index;       // 边的序号
    bool selected;   // 表示是否已被选中构建最小生成树
};

void insertSort(vector<ENode*>& g) {
    for(int i = 1; i < g.size(); i++) {
        int cur = i;
        int pre = cur - 1;
        while(cur > 0 && (g[cur]->weight < g[pre]->weight)) {
            // 交换两个节点的位置
            ENode* temp = g[pre];
            g[pre] = g[cur];
            g[cur] = temp;
            // 更新cur指针，确保其一直指向当前所要操作的那个节点
            cur--;
            pre--;
        }
    }
}

bool dfs(const vector<ENode*>& g, vector<bool>& visited, int cur) {
    // 判断当前连通分支中有无回路
    if(visited[cur]) return true;
    visited[cur] = true;   // 访问当前节点
    // 搜索其邻接节点
    for(int j= 0; j < g.size(); j++) {
        if(g[j]->begin == cur && g[j]->selected) {
            if(dfs(g, visited, g[j]->end)) {
                return true;
            }
        }
    }
    return false;
}

bool isValid(const vector<ENode*>& g, int v) {
    // 判断有无环
    bool ans = true;
    vector<bool> visited(v, false);   // 节点是否被访问                
    for(int i = 0; i < v; i++) {
        if(!visited[i]) {
            if(dfs(g, visited, i)) {
                ans = false;   // 发现回路 
                break;
            }
        }
    }
    return ans;
}

void kruskal(vector<ENode*>& g, int v) {
    int current = 0;   // 当前生成树中边的个数
    for(int i = 0; i < g.size(); i++) {
        if(current == v-1) break;
        // 尝试将g[i]加入图中
        g[i]->selected = true;
        if(isValid(g, v)) {
            current++;
        } else {
            g[i]->selected = false;
        }
    }
}

int main() {
    int v, e;
    cin >> v >> e;
    // 创建边的集合
    // 注意图是无向图
    vector<ENode*> graph;         
    for(int i = 0; i < e; i++) {
        // 创建一条有向边
        ENode* node = new ENode();
        cin >> node->index >> node->begin >> node->end >> node->weight;
        node->selected = false; 
        graph.push_back(node);
        // 创建一条与其方向相反的有向边
        ENode* back = new ENode();
        back->begin = node->end;
        back->end = node->begin;
        back->index = node->index;
        back->weight = node->weight;
        back->selected = false;
        graph.push_back(back);
    }
    // 将边数组按照权值从小到大排序
    insertSort(graph);
    kruskal(graph, v);
    // 打印结果
    int sum = 0;
    vector<int> list;
    for(int k = 0; k < graph.size(); k++) {
        if(graph[k]->selected == false) continue;
        sum += graph[k]->weight;
        list.push_back(graph[k]->index);
    }
    sort(list.begin(), list.end());
    cout << sum << endl;
    for(int j = 0; j < list.size(); j++) {
        cout << list[j] << " ";
    }
}