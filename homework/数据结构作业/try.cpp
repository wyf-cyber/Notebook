#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
using namespace std;

struct ENode {
    int end;
    int weight;
    ENode* next;
};

struct HNode {
    ENode* next;
};

void dfs(vector<HNode*> g) {
    vector<bool> visited(g.size(), false);
    stack<int> s;
    s.push(0);
    while(!s.empty()) {
        int node = s.top();
        s.pop();
        if(visited[node]) continue;
        visited[node] = true;
        ENode* p = g[node]->next;
        while(p) {
            if(!visited[p->end]) {
                s.push(p->end);
            } 
            p = p->next;
        } 
    }
}
void bfs(vector<HNode*> g) {
    queue<int> q;
    q.push(0);
    vector<bool> visited(g.size(), false);
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        if(visited[node]) continue;
        visited[node] = true;
        ENode* cur = g[node]->next;
        while(cur) {
            if(!visited[cur->end]) {
                q.push(cur->end);
            } 
            cur = cur->next;
        }
    }
}