/**
 * 点分治 (Centroid Decomposition)
 * 功能：树上路径问题
 * 复杂度：O(n log n)
 */
#include <bits/stdc++.h>
using namespace std;

struct CentroidDecomposition {
    int n;
    vector<vector<int>> g;
    vector<bool> removed;
    vector<int> siz;

    CentroidDecomposition(int n) : n(n), g(n), removed(n), siz(n) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int calcSize(int u, int p) {
        siz[u] = 1;
        for (int v : g[u]) {
            if (v != p && !removed[v]) {
                siz[u] += calcSize(v, u);
            }
        }
        return siz[u];
    }

    int findCentroid(int u, int p, int n) {
        for (int v : g[u]) {
            if (v != p && !removed[v] && siz[v] > n / 2) {
                return findCentroid(v, u, n);
            }
        }
        return u;
    }

    void solve(int u) {
        int n = calcSize(u, -1);
        int c = findCentroid(u, -1, n);

        removed[c] = true;

        // 在这里处理以c为根的子树
        // 例如：统计路径、更新答案等

        for (int v : g[c]) {
            if (!removed[v]) {
                solve(v);
            }
        }
    }
};
