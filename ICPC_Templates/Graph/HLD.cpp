/**
 * 树链剖分 (Heavy Light Decomposition)
 * 功能：树上路径查询与修改
 * 复杂度：O(log n) 每次操作
 */
#include <bits/stdc++.h>
using namespace std;

struct HLD {
    int n, cnt;
    vector<int> siz, top, dep, parent, in, out, seq;
    vector<vector<int>> g;

    HLD() {}
    HLD(int n) : n(n), cnt(0) {
        siz.resize(n);
        top.resize(n);
        dep.resize(n);
        parent.resize(n);
        in.resize(n);
        out.resize(n);
        seq.resize(n);
        g.resize(n);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int fa) {
        parent[u] = fa;
        siz[u] = 1;
        for (int& v : g[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            dfs1(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[g[u][0]] || g[u][0] == fa) {
                swap(v, g[u][0]);
            }
        }
    }

    void dfs2(int u, int t) {
        top[u] = t;
        in[u] = cnt++;
        seq[in[u]] = u;
        if (g[u].empty() || (g[u].size() == 1 && g[u][0] == parent[u])) {
            out[u] = cnt;
            return;
        }
        for (int v : g[u]) {
            if (v == parent[u]) continue;
            dfs2(v, v == g[u][0] ? t : v);
        }
        out[u] = cnt;
    }

    void build(int root = 0) {
        dep[root] = 0;
        dfs1(root, -1);
        dfs2(root, root);
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }
            u = parent[top[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    // 对路径 u-v 应用操作（需要配合数据结构使用）
    template <typename F>
    void pathApply(int u, int v, F&& f) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }
            f(in[top[u]], in[u] + 1);
            u = parent[top[u]];
        }
        if (dep[u] > dep[v]) {
            swap(u, v);
        }
        f(in[u], in[v] + 1);
    }

    // 子树操作区间为 [in[u], out[u])
};
