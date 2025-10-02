// Virtual Tree (虚树)
// 功能：压缩树结构，只保留关键节点及其LCA
// 复杂度：O(k log k)，其中k是关键节点数
// 应用：树上DP优化、关键节点的树形问题

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 10;
const long long INF = 1e18;

struct VirtualTree {
    vector<int> g[MAXN];   // 原树
    vector<int> vg[MAXN];  // 虚树
    long long edge[MAXN];  // 虚树边权

    int dep[MAXN], fa[MAXN];  // 深度，父节点
    int sz[MAXN], son[MAXN];  // 子树大小，重儿子
    int top[MAXN];            // 重链顶端
    long long dist[MAXN];     // 到根距离

    int dfn[MAXN], dfs_clock;  // DFS序
    bool is_key[MAXN];         // 是否为关键节点
    long long dp[MAXN];        // DP数组

    // 预处理：树链剖分
    void dfs1(int u, int f) {
        dep[u] = dep[f] + 1;
        fa[u] = f;
        sz[u] = 1;

        for (int v : g[u]) {
            if (v == f) continue;
            dfs1(v, u);
            sz[u] += sz[v];
            if (sz[v] > sz[son[u]]) {
                son[u] = v;
            }
        }
    }

    void dfs2(int u, int t) {
        dfn[u] = ++dfs_clock;
        top[u] = t;

        if (son[u]) dfs2(son[u], t);

        for (int v : g[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }
    }

    // LCA查询
    int lca(int x, int y) {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            x = fa[top[x]];
        }
        return dep[x] < dep[y] ? x : y;
    }

    // 构建虚树
    vector<int> build_virtual_tree(vector<int>& key_nodes) {
        // 清空虚树
        for (int u : key_nodes) {
            vg[u].clear();
            is_key[u] = true;
            dp[u] = 0;
        }

        // 按DFS序排序
        sort(key_nodes.begin(), key_nodes.end(),
             [&](int x, int y) { return dfn[x] < dfn[y]; });

        // 加入关键节点间的LCA
        int k = key_nodes.size();
        for (int i = 1; i < k; i++) {
            int l = lca(key_nodes[i - 1], key_nodes[i]);
            if (!is_key[l]) {
                is_key[l] = true;
                key_nodes.push_back(l);
                dp[l] = 0;
            }
        }

        // 加入根节点
        if (!is_key[1]) {
            is_key[1] = true;
            key_nodes.push_back(1);
            dp[1] = 0;
        }

        // 重新排序
        sort(key_nodes.begin(), key_nodes.end(),
             [&](int x, int y) { return dfn[x] < dfn[y]; });

        // 用栈构建虚树
        stack<int> stk;
        stk.push(key_nodes[0]);

        for (int i = 1; i < (int)key_nodes.size(); i++) {
            int u = key_nodes[i];
            while (dfn[u] > dfn[stk.top()] + sz[stk.top()] - 1) {
                stk.pop();
            }

            int p = stk.top();
            vg[p].push_back(u);
            edge[u] = dist[u] - dist[p];  // 虚树边权
            stk.push(u);
        }

        return key_nodes;
    }

    // 在虚树上DP（示例：最小边权覆盖）
    void solve_dp(int u) {
        if (vg[u].empty()) {
            // 叶节点
            dp[u] = is_key[u] ? edge[u] : 0;
            return;
        }

        for (int v : vg[u]) {
            solve_dp(v);
            dp[u] += dp[v];
        }

        // 根据具体问题修改DP转移
        if (is_key[u]) {
            dp[u] = min(dp[u], edge[u]);
        }
    }

    // 清理标记
    void clear(vector<int>& nodes) {
        for (int u : nodes) {
            is_key[u] = false;
            vg[u].clear();
        }
    }

    void init(int n) {
        dfs1(1, 0);
        dfs2(1, 1);
    }
};

// 使用示例
void solve() {
    int n;
    cin >> n;

    VirtualTree vt;

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        vt.g[u].push_back(v);
        vt.g[v].push_back(u);
    }

    vt.init(n);

    int q;
    cin >> q;
    while (q--) {
        int k;
        cin >> k;

        vector<int> key_nodes(k);
        for (int i = 0; i < k; i++) {
            cin >> key_nodes[i];
        }

        auto nodes = vt.build_virtual_tree(key_nodes);
        vt.solve_dp(1);

        cout << vt.dp[1] << "\n";

        vt.clear(nodes);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
