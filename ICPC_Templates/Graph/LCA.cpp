/**
 * 最近公共祖先 (Lowest Common Ancestor)
 * 方法：倍增法
 * 复杂度：预处理 O(n log n)，查询 O(log n)
 */
#include <bits/stdc++.h>
using namespace std;

struct LCA {
    int n, lg;
    vector<vector<int>> g, up;
    vector<int> dep;

    LCA() {}
    LCA(int n) : n(n), lg(__lg(n) + 1) {
        g.resize(n);
        up.assign(lg, vector<int>(n));
        dep.resize(n);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa) {
        up[0][u] = fa;
        for (int i = 1; i < lg; i++) {
            up[i][u] = up[i - 1][up[i - 1][u]];
        }
        for (int v : g[u]) {
            if (v != fa) {
                dep[v] = dep[u] + 1;
                dfs(v, u);
            }
        }
    }

    void build(int root = 0) {
        dep[root] = 0;
        dfs(root, root);
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) {
            swap(u, v);
        }

        for (int i = lg - 1; i >= 0; i--) {
            if (dep[up[i][u]] >= dep[v]) {
                u = up[i][u];
            }
        }

        if (u == v) {
            return u;
        }

        for (int i = lg - 1; i >= 0; i--) {
            if (up[i][u] != up[i][v]) {
                u = up[i][u];
                v = up[i][v];
            }
        }

        return up[0][u];
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }
};
