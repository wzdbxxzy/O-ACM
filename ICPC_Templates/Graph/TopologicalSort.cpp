/**
 * 拓扑排序 (Topological Sort)
 * 功能：有向无环图的线性序
 * 复杂度：O(n + m)
 */
#include <bits/stdc++.h>
using namespace std;

// Kahn算法（BFS）
vector<int> topologicalSort(int n, const vector<vector<int>>& g) {
    vector<int> in(n);
    for (int u = 0; u < n; u++) {
        for (int v : g[u]) {
            in[v]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (in[i] == 0) {
            q.push(i);
        }
    }

    vector<int> res;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        res.push_back(u);

        for (int v : g[u]) {
            if (--in[v] == 0) {
                q.push(v);
            }
        }
    }

    if (res.size() != n) {
        return {};  // 存在环
    }
    return res;
}

// DFS版本
struct TopoSort {
    int n;
    vector<vector<int>> g;
    vector<int> order;
    vector<int> color;  // 0:未访问, 1:访问中, 2:已完成
    bool hasCycle;

    TopoSort(int n) : n(n), g(n), color(n), hasCycle(false) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    void dfs(int u) {
        color[u] = 1;
        for (int v : g[u]) {
            if (color[v] == 1) {
                hasCycle = true;
                return;
            }
            if (color[v] == 0) {
                dfs(v);
            }
        }
        color[u] = 2;
        order.push_back(u);
    }

    vector<int> sort() {
        for (int i = 0; i < n; i++) {
            if (color[i] == 0) {
                dfs(i);
            }
        }
        if (hasCycle) return {};
        reverse(order.begin(), order.end());
        return order;
    }
};
