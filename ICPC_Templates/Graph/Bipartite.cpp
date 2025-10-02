/**
 * 二分图 (Bipartite Graph)
 * 功能：判定二分图、最大匹配、最小覆盖等
 * 复杂度：O(nm)
 */
#include <bits/stdc++.h>
using namespace std;

// 二分图判定
bool isBipartite(int n, const vector<vector<int>>& g) {
    vector<int> color(n, -1);

    function<bool(int, int)> dfs = [&](int u, int c) -> bool {
        color[u] = c;
        for (int v : g[u]) {
            if (color[v] == -1) {
                if (!dfs(v, c ^ 1)) {
                    return false;
                }
            } else if (color[v] == c) {
                return false;
            }
        }
        return true;
    };

    for (int i = 0; i < n; i++) {
        if (color[i] == -1) {
            if (!dfs(i, 0)) {
                return false;
            }
        }
    }
    return true;
}

// 二分图最大匹配（匈牙利算法）
struct Hungarian {
    int n, m;
    vector<vector<int>> g;
    vector<int> match;
    vector<bool> vis;

    Hungarian(int n, int m) : n(n), m(m), g(n), match(m, -1) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    bool dfs(int u) {
        for (int v : g[u]) {
            if (vis[v]) continue;
            vis[v] = true;
            if (match[v] == -1 || dfs(match[v])) {
                match[v] = u;
                return true;
            }
        }
        return false;
    }

    int maxMatch() {
        int res = 0;
        for (int i = 0; i < n; i++) {
            vis.assign(m, false);
            if (dfs(i)) {
                res++;
            }
        }
        return res;
    }
};

// KM算法（二分图最大权匹配）
struct KM {
    static constexpr int INF = 1e9;
    int n;
    vector<vector<int>> w;
    vector<int> la, lb;
    vector<int> match;
    vector<bool> va, vb;
    vector<int> delta;

    KM(int n) : n(n), w(n, vector<int>(n)), la(n), lb(n), match(n, -1) {}

    void addEdge(int u, int v, int weight) { w[u][v] = max(w[u][v], weight); }

    bool dfs(int u) {
        va[u] = true;
        for (int v = 0; v < n; v++) {
            if (vb[v]) continue;
            int t = la[u] + lb[v] - w[u][v];
            if (t == 0) {
                vb[v] = true;
                if (match[v] == -1 || dfs(match[v])) {
                    match[v] = u;
                    return true;
                }
            } else {
                delta[v] = min(delta[v], t);
            }
        }
        return false;
    }

    int maxWeight() {
        for (int i = 0; i < n; i++) {
            la[i] = *max_element(w[i].begin(), w[i].end());
        }

        for (int i = 0; i < n; i++) {
            delta.assign(n, INF);
            while (true) {
                va.assign(n, false);
                vb.assign(n, false);
                if (dfs(i)) break;

                int d = INF;
                for (int j = 0; j < n; j++) {
                    if (!vb[j]) d = min(d, delta[j]);
                }

                for (int j = 0; j < n; j++) {
                    if (va[j]) la[j] -= d;
                    if (vb[j])
                        lb[j] += d;
                    else
                        delta[j] -= d;
                }
            }
        }

        int res = 0;
        for (int i = 0; i < n; i++) {
            if (match[i] != -1) {
                res += w[match[i]][i];
            }
        }
        return res;
    }
};
