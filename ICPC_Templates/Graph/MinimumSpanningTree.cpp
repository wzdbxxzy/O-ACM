/**
 * 最小生成树 (Minimum Spanning Tree)
 * Kruskal算法和Prim算法
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Kruskal算法
struct Edge {
    int u, v;
    ll w;
    bool operator<(const Edge& e) const { return w < e.w; }
};

struct DSU {
    vector<int> f;

    DSU(int n) {
        f.resize(n);
        iota(f.begin(), f.end(), 0);
    }

    int find(int x) {
        while (x != f[x]) {
            x = f[x] = f[f[x]];
        }
        return x;
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        f[y] = x;
        return true;
    }
};

// Kruskal算法求最小生成树
pair<ll, vector<Edge>> kruskal(int n, vector<Edge> edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);

    ll cost = 0;
    vector<Edge> mst;

    for (auto& e : edges) {
        if (dsu.merge(e.u, e.v)) {
            cost += e.w;
            mst.push_back(e);
            if (mst.size() == n - 1) {
                break;
            }
        }
    }

    if (mst.size() != n - 1) {
        return {-1, {}};  // 图不连通
    }

    return {cost, mst};
}

// Prim算法
ll prim(int n, const vector<vector<pair<int, ll>>>& g) {
    vector<bool> vis(n);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;

    ll cost = 0;
    int cnt = 0;

    pq.push({0, 0});

    while (!pq.empty() && cnt < n) {
        auto [w, u] = pq.top();
        pq.pop();

        if (vis[u]) continue;

        vis[u] = true;
        cost += w;
        cnt++;

        for (auto [v, w] : g[u]) {
            if (!vis[v]) {
                pq.push({w, v});
            }
        }
    }

    if (cnt != n) {
        return -1;  // 图不连通
    }

    return cost;
}

// 次小生成树
ll secondMST(int n, vector<Edge> edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n);

    vector<Edge> mst;
    ll mstCost = 0;

    // 求最小生成树
    for (auto& e : edges) {
        if (dsu.merge(e.u, e.v)) {
            mst.push_back(e);
            mstCost += e.w;
        }
    }

    ll ans = LLONG_MAX;

    // 枚举删除MST中的每一条边
    for (int i = 0; i < mst.size(); i++) {
        DSU dsu2(n);
        ll cost = 0;
        int cnt = 0;

        for (auto& e : edges) {
            if (e.u == mst[i].u && e.v == mst[i].v && e.w == mst[i].w) {
                continue;
            }
            if (dsu2.merge(e.u, e.v)) {
                cost += e.w;
                cnt++;
            }
        }

        if (cnt == n - 1) {
            ans = min(ans, cost);
        }
    }

    return ans;
}
