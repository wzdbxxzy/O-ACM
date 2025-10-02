/**
 * 欧拉回路/欧拉路径
 * 功能：找到遍历所有边恰好一次的路径
 * 复杂度：O(m)
 */
#include <bits/stdc++.h>
using namespace std;

// 无向图欧拉回路
struct EulerCircuitUndirected {
    int n;
    vector<map<int, int>> g;
    vector<int> path;

    EulerCircuitUndirected(int n) : n(n), g(n) {}

    void addEdge(int u, int v) {
        g[u][v]++;
        g[v][u]++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, cnt] = *g[u].begin();
            if (cnt == 0) {
                g[u].erase(v);
                continue;
            }
            g[u][v]--;
            g[v][u]--;
            if (g[u][v] == 0) g[u].erase(v);
            if (g[v][u] == 0) g[v].erase(u);
            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> findCircuit(int start = 0) {
        // 检查是否存在欧拉回路
        for (int i = 0; i < n; i++) {
            int deg = 0;
            for (auto [v, cnt] : g[i]) deg += cnt;
            if (deg % 2 != 0) return {};  // 不存在欧拉回路
        }

        dfs(start);
        reverse(path.begin(), path.end());
        return path;
    }
};

// 有向图欧拉回路
struct EulerCircuitDirected {
    int n;
    vector<vector<int>> g;
    vector<int> cur;
    vector<int> path;

    EulerCircuitDirected(int n) : n(n), g(n), cur(n) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    void dfs(int u) {
        while (cur[u] < g[u].size()) {
            int v = g[u][cur[u]++];
            dfs(v);
        }
        path.push_back(u);
    }

    vector<int> findCircuit(int start = 0) {
        // 检查入度=出度
        vector<int> in(n), out(n);
        for (int u = 0; u < n; u++) {
            out[u] = g[u].size();
            for (int v : g[u]) in[v]++;
        }
        for (int i = 0; i < n; i++) {
            if (in[i] != out[i]) return {};
        }

        dfs(start);
        reverse(path.begin(), path.end());
        return path;
    }
};
