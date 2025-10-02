// Point Biconnected Component (点双连通分量)
// 功能：找出所有点双连通分量和割点
// 复杂度：O(n + m)
// 应用：无向图连通性、圆方树构建

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;

struct PointBCC {
    vector<int> g[MAXN];
    int dfn[MAXN], low[MAXN], dfs_clock;
    int stk[MAXN], top;

    bool is_cut[MAXN];      // 是否为割点
    int bcc_cnt;            // 点双连通分量数量
    vector<int> bcc[MAXN];  // 每个点双的节点集合
    int belong[MAXN];       // 每个节点所属的点双（可能属于多个）

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void tarjan(int u, int fa) {
        dfn[u] = low[u] = ++dfs_clock;
        stk[++top] = u;

        int children = 0;  // 子树数量（用于判断根是否为割点）

        for (int v : g[u]) {
            if (v == fa) continue;

            if (!dfn[v]) {
                children++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);

                // 发现点双连通分量
                if (low[v] >= dfn[u]) {
                    is_cut[u] = true;
                    bcc_cnt++;
                    bcc[bcc_cnt].clear();

                    int x;
                    do {
                        x = stk[top--];
                        bcc[bcc_cnt].push_back(x);
                    } while (x != v);

                    bcc[bcc_cnt].push_back(u);
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }

        // 根节点特殊判断
        if (fa == -1) {
            is_cut[u] = (children > 1);
        }
    }

    void find_bcc(int n) {
        memset(dfn, 0, sizeof(dfn));
        memset(low, 0, sizeof(low));
        memset(is_cut, false, sizeof(is_cut));
        dfs_clock = 0;
        top = 0;
        bcc_cnt = 0;

        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) {
                tarjan(i, -1);
            }
        }
    }

    // 构建圆方树（Block-Cut Tree）
    // 圆点：原图的点，方点：点双连通分量
    vector<int> tree[MAXN * 2];  // 圆方树（最多n个圆点 + bcc_cnt个方点）
    int square_node_start;       // 方点编号起始位置

    void build_block_cut_tree(int n) {
        square_node_start = n + 1;

        for (int i = 1; i <= bcc_cnt; i++) {
            int square = square_node_start + i - 1;
            for (int u : bcc[i]) {
                tree[u].push_back(square);
                tree[square].push_back(u);
            }
        }
    }

    void clear(int n) {
        for (int i = 1; i <= n + bcc_cnt; i++) {
            g[i].clear();
            tree[i].clear();
            bcc[i].clear();
        }
    }
};

// 使用示例
void solve() {
    int n, m;
    cin >> n >> m;

    PointBCC pb;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        pb.add_edge(u, v);
    }

    pb.find_bcc(n);

    // 输出割点
    cout << "Cut vertices: ";
    for (int i = 1; i <= n; i++) {
        if (pb.is_cut[i]) {
            cout << i << " ";
        }
    }
    cout << "\n";

    // 输出点双连通分量
    cout << "Number of BCCs: " << pb.bcc_cnt << "\n";
    for (int i = 1; i <= pb.bcc_cnt; i++) {
        cout << "BCC " << i << ": ";
        for (int u : pb.bcc[i]) {
            cout << u << " ";
        }
        cout << "\n";
    }

    // 构建圆方树
    pb.build_block_cut_tree(n);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
