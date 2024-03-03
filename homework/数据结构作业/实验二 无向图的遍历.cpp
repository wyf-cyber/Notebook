#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

struct node {
    int val;        // 边的另一个节点的序号
    node* next;     // 指向与该节点相连的下一个节点
    node(int value) : val(value), next(nullptr) {} 
    node() : val(0), next(nullptr){}
};
// 深度优先遍历
void dfs(vector<node*>& graph) {
    stack<int> s; 
    vector<bool> visited(graph.size(), false);  // 创建闭表
    s.push(0); // 从0号节点开始

    while (!s.empty()) {
        int current = s.top();
        s.pop();
        // 如果当前节点未被访问，读取该节点的值并将它的邻接节点入栈
        if (!visited[current]) {
            cout << current << " ";  // 读取当前节点
            visited[current] = true;

            // 遍历所有邻接节点并将尚未被遍历的节点全部加入临时数组中
            node* neighbor = graph[current]->next;       
            vector<int> unvisitedNeighbors;   // 用于存储当前节点所有未被访问的邻接节点的编号
            while (neighbor != nullptr) {
                if (!visited[neighbor->val]) {
                    unvisitedNeighbors.push_back(neighbor->val);
                }
                neighbor = neighbor->next;
            }
            // 调整邻接节点压栈的顺序
            sort(unvisitedNeighbors.begin(), unvisitedNeighbors.end());
            // 值较大的节点将会优先被压入栈中
            for (int k = unvisitedNeighbors.size() - 1; k >= 0; k--) {
                s.push(unvisitedNeighbors[k]);
            }
        }
    }
}

void bfs(vector<node*>& graph, vector<bool>& visited) {
    queue<int> level;
    level.push(0);
    while(!level.empty()) {
        // 每一次while循环对应一圈
        int n = level.size();
  
        for(int i = 0; i < n; i++) {
            // 处理一个节点      
            vector<int> nodes;
            int temp = level.front();
            level.pop();
            if(visited[temp]) continue;
            visited[temp] = true;
            cout << temp << " ";
            // 将当前节点的所有未访问的邻接节点加入nodes[]
            node* neighbor = graph[temp]->next;
            while(neighbor != nullptr) {
                if(!visited[neighbor->val]) {
                    nodes.push_back(neighbor->val);
                }
                neighbor = neighbor->next;
            }
            // 将临时数组中存储的邻接节点入队，序号小的节点优先入队
            sort(nodes.begin(), nodes.end());
            for(int j = 0; j < nodes.size(); j++) {
                level.push(nodes[j]);
            }
        }
    }
}

int main() {
    int v = 0;
    int e = 0;
    cin >> v >> e;
    vector<node*> graph;     // 0 表示两点之间没有边相连
    // 初始化邻接表数组
    for(int j = 0; j < v; j++) {
        node* newnode = new node(j);
        graph.push_back(newnode);
    }
    // 录入边的信息
    for(int i = 0; i < e; i++) {
        int x = 0, y = 0;
        cin >> x >> y;
        node* nodeOne = new node(x);
        node* nodeTwo = new node(y);
        // 将新节点插入以graph[x] 为头结点的链表
        // 头插法
        nodeOne->next = graph[y]->next;
        nodeTwo->next = graph[x]->next;
        graph[y]->next = nodeOne;
        graph[x]->next = nodeTwo;
    }
    // 读取要删除的顶点编号
    int delNode = 1;
    cin >> delNode;
    // 深度优先遍历
    dfs(graph);
    cout << endl;
    // 删除指定节点，通过将该节点加入闭表中实现
    vector<bool> visited(v, false);
    visited[delNode] = true;
    bfs(graph, visited);
}