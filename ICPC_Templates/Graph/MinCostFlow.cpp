/**
 * 最小费用最大流 (Minimum Cost Maximum Flow)
 * 功能：在保证最大流的前提下，使费用最小
 * 复杂度：O(nmf)
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class T>
struct MinCostFlow {
    struct Edge {
        int to;
        T cap;
        T cost;
        Edge(int to_, T cap_, T cost_) : to(to_), cap(cap_), cost(cost_) {}
    };

    int n;
    vector<Edge> e;
    vector<vector<int>> g;
    vector<T> h, dis;
    vector<int> pre;

    bool dijkstra(int s, int t) {
        dis.assign(n, numeric_limits<T>::max());
        pre.assign(n, -1);
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> que;
        dis[s] = 0;
        que.emplace(0, s);
        while (!que.empty()) {
            auto [d, u] = que.top();
            que.pop();
            if (dis[u] != d) {
                continue;
            }
            for (int i : g[u]) {
                int v = e[i].to;
                T cap = e[i].cap;
                T cost = e[i].cost;
                if (cap > 0 && dis[v] > d + h[u] - h[v] + cost) {
                    dis[v] = d + h[u] - h[v] + cost;
                    pre[v] = i;
                    que.emplace(dis[v], v);
                }
            }
        }
        return dis[t] != numeric_limits<T>::max();
    }

    MinCostFlow() {}
    MinCostFlow(int n_) { init(n_); }

    void init(int n_) {
        n = n_;
        e.clear();
        g.assign(n, {});
    }

    void addEdge(int u, int v, T cap, T cost) {
        g[u].push_back(e.size());
        e.emplace_back(v, cap, cost);
        g[v].push_back(e.size());
        e.emplace_back(u, 0, -cost);
    }

    pair<T, T> flow(int s, int t) {
        T flow = 0;
        T cost = 0;
        h.assign(n, 0);
        while (dijkstra(s, t)) {
            for (int i = 0; i < n; ++i) {
                h[i] += dis[i];
            }
            T aug = numeric_limits<T>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                aug = min(aug, e[pre[i]].cap);
            }
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                e[pre[i]].cap -= aug;
                e[pre[i] ^ 1].cap += aug;
            }
            flow += aug;
            cost += aug * h[t];
        }
        return {flow, cost};
    }
};
