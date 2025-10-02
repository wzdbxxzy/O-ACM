/**
 * 最短路算法
 * Dijkstra, Bellman-Ford, SPFA, Floyd
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Dijkstra - 适用于非负权图
// 复杂度：O((n + m) log n)
vector<ll> dijkstra(int n, int s, const vector<vector<pair<int, ll>>>& g) {
    vector<ll> dis(n, LLONG_MAX);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    dis[s] = 0;
    pq.emplace(0, s);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dis[u]) continue;

        for (auto [v, w] : g[u]) {
            if (dis[u] + w < dis[v]) {
                dis[v] = dis[u] + w;
                pq.emplace(dis[v], v);
            }
        }
    }
    return dis;
}

// SPFA - 适用于有负权边的图（但无负环）
// 平均复杂度：O(km)，最坏 O(nm)
vector<ll> spfa(int n, int s, const vector<vector<pair<int, ll>>>& g) {
    vector<ll> dis(n, LLONG_MAX);
    vector<bool> inq(n);
    queue<int> q;

    dis[s] = 0;
    q.push(s);
    inq[s] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;

        for (auto [v, w] : g[u]) {
            if (dis[u] != LLONG_MAX && dis[u] + w < dis[v]) {
                dis[v] = dis[u] + w;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return dis;
}

// Floyd - 全源最短路
// 复杂度：O(n^3)
void floyd(vector<vector<ll>>& dis, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dis[i][k] != LLONG_MAX && dis[k][j] != LLONG_MAX) {
                    dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
                }
            }
        }
    }
}
