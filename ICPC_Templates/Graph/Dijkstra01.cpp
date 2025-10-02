// 01BFS / Dijkstra for 0-1 edge weights
// 功能：边权只有0和1的最短路
// 复杂度：O(V + E)
// 应用：特殊最短路问题，网格图最短路

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;
const int INF = 1e9;

struct Dijkstra01 {
    struct Edge {
        int to, w;  // w只能是0或1
    };

    vector<Edge> g[MAXN];
    int dist[MAXN];
    bool vis[MAXN];
    int n;

    void add_edge(int u, int v, int w) { g[u].push_back({v, w}); }

    // 单源最短路
    void solve(int s) {
        fill(dist, dist + n + 1, INF);
        fill(vis, vis + n + 1, false);

        deque<int> q;
        dist[s] = 0;
        q.push_back(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop_front();

            if (vis[u]) continue;
            vis[u] = true;

            for (auto [v, w] : g[u]) {
                int new_dist = dist[u] + w;
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    if (w == 0) {
                        q.push_front(v);  // 0边放队首
                    } else {
                        q.push_back(v);  // 1边放队尾
                    }
                }
            }
        }
    }

    // 多源最短路
    void solve(vector<int>& sources) {
        fill(dist, dist + n + 1, INF);
        fill(vis, vis + n + 1, false);

        deque<int> q;
        for (int s : sources) {
            dist[s] = 0;
            q.push_back(s);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop_front();

            if (vis[u]) continue;
            vis[u] = true;

            for (auto [v, w] : g[u]) {
                int new_dist = dist[u] + w;
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    if (w == 0) {
                        q.push_front(v);
                    } else {
                        q.push_back(v);
                    }
                }
            }
        }
    }
};

// 网格图专用版本（常见应用）
struct Grid01BFS {
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    int n, m;
    vector<vector<int>> grid;  // 0表示可通过，1表示障碍
    vector<vector<int>> dist;

    Grid01BFS(int n_, int m_) : n(n_), m(m_) {
        grid.assign(n, vector<int>(m, 0));
        dist.assign(n, vector<int>(m, INF));
    }

    bool valid(int x, int y) { return x >= 0 && x < n && y >= 0 && y < m; }

    // 从(sx, sy)到所有点的最短路
    // 允许翻墙cost=1，走空地cost=0
    void solve(int sx, int sy) {
        dist.assign(n, vector<int>(m, INF));
        deque<pair<int, int>> q;

        dist[sx][sy] = 0;
        q.push_back({sx, sy});

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop_front();

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (!valid(nx, ny)) continue;

                int cost = grid[nx][ny];  // 0或1
                int new_dist = dist[x][y] + cost;

                if (new_dist < dist[nx][ny]) {
                    dist[nx][ny] = new_dist;
                    if (cost == 0) {
                        q.push_front({nx, ny});
                    } else {
                        q.push_back({nx, ny});
                    }
                }
            }
        }
    }
};

// 使用示例1：普通图
void example1() {
    int n, m;
    cin >> n >> m;

    Dijkstra01 d;
    d.n = n;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;  // w只能是0或1
        d.add_edge(u, v, w);
    }

    int s;
    cin >> s;
    d.solve(s);

    for (int i = 1; i <= n; i++) {
        cout << d.dist[i] << " ";
    }
    cout << "\n";
}

// 使用示例2：网格图
void example2() {
    int n, m;
    cin >> n >> m;

    Grid01BFS grid(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid.grid[i][j];
        }
    }

    int sx, sy, tx, ty;
    cin >> sx >> sy >> tx >> ty;

    grid.solve(sx, sy);

    cout << grid.dist[tx][ty] << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // example1();
    // example2();

    return 0;
}
