// Centroid Decomposition Tree (点分树)
// 功能：动态维护树上距离信息
// 复杂度：构建 O(n log n)，查询 O(log^2 n)
// 应用：动态查询树上k距离内点权和

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;

struct BIT {
    vector<long long> c;
    int n;

    void init(int _n) {
        n = _n;
        c.resize(n + 3, 0);
    }

    void add(int pos, long long val) {
        pos++;  // 防止距离为0的情况
        for (int i = pos; i <= n; i += i & -i) {
            c[i] += val;
        }
    }

    long long query(int x) {
        x++;
        x = min(x, n);
        long long res = 0;
        for (int i = x; i > 0; i -= i & -i) {
            res += c[i];
        }
        return res;
    }
};

struct CentroidTree {
    vector<int> g[MAXN];  // 原树
    BIT tree1[MAXN];      // 从重心到子树的距离统计
    BIT tree2[MAXN];      // 从父重心到子树的距离统计

    int dep[MAXN];     // 原树深度
    int fa[MAXN];      // 点分树父节点
    int siz[MAXN];     // 子树大小
    int maxsiz[MAXN];  // 最大子树大小
    bool vis[MAXN];    // 是否已被分治

    int n;
    int root;
    int total_size;

    // LCA预处理
    int f[MAXN][20], top[MAXN], wson[MAXN], treesiz[MAXN];

    void dfs_lca(int u, int fa_) {
        treesiz[u] = 1;
        dep[u] = dep[f[u][0] = fa_] + 1;
        for (int v : g[u]) {
            if (v == fa_) continue;
            dfs_lca(v, u);
            treesiz[u] += treesiz[v];
            if (treesiz[wson[u]] < treesiz[v]) {
                wson[u] = v;
            }
        }
    }

    void dfs_chain(int u, int topf) {
        top[u] = topf;
        if (!wson[u]) return;
        dfs_chain(wson[u], topf);
        for (int v : g[u]) {
            if (v == f[u][0] || v == wson[u]) continue;
            dfs_chain(v, v);
        }
    }

    void init_lca(int root_) {
        dfs_lca(root_, 0);
        dfs_chain(root_, root_);
        for (int j = 1; j < 20; j++) {
            for (int i = 1; i <= n; i++) {
                f[i][j] = f[f[i][j - 1]][j - 1];
            }
        }
    }

    int lca(int x, int y) {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            x = f[top[x]][0];
        }
        return dep[x] < dep[y] ? x : y;
    }

    int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }

    // 找重心
    void get_root(int u, int fa_) {
        siz[u] = 1;
        maxsiz[u] = 0;
        for (int v : g[u]) {
            if (v == fa_ || vis[v]) continue;
            get_root(v, u);
            siz[u] += siz[v];
            maxsiz[u] = max(maxsiz[u], siz[v]);
        }
        maxsiz[u] = max(maxsiz[u], total_size - siz[u]);
        if (maxsiz[u] < maxsiz[root]) {
            root = u;
        }
    }

    // 点分治建树
    void build(int u, int fa_) {
        int now_size = total_size;
        vis[u] = true;
        fa[u] = fa_;

        // 初始化BIT
        tree1[u].init(total_size / 2 + 1);
        tree2[u].init(total_size + 1);

        for (int v : g[u]) {
            if (vis[v]) continue;
            // 计算子树大小
            total_size = siz[v] > siz[u] ? now_size - siz[u] : siz[v];
            maxsiz[root = 0] = 1e9;
            get_root(v, 0);
            build(root, u);
        }
    }

    // 单点修改
    void modify(int x, long long val) {
        tree1[x].add(0, val);
        for (int i = x; fa[i]; i = fa[i]) {
            int d = dist(x, fa[i]);
            tree1[fa[i]].add(d, val);
            tree2[i].add(d, val);
        }
    }

    // 查询距离x不超过k的点权和
    long long query(int x, int k) {
        long long res = tree1[x].query(k);
        for (int i = x; fa[i]; i = fa[i]) {
            int d = dist(x, fa[i]);
            if (d > k) continue;
            res += tree1[fa[i]].query(k - d);
            res -= tree2[i].query(k - d);
        }
        return res;
    }

    void init(int n_) {
        n = n_;
        total_size = n;
        maxsiz[root = 0] = 1e9;
        get_root(1, 0);
        init_lca(root);
        build(root, 0);
    }
};

// 使用示例
void solve() {
    int n, q;
    cin >> n >> q;

    CentroidTree ct;
    ct.n = n;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        ct.g[u].push_back(v);
        ct.g[v].push_back(u);
    }

    ct.init(n);

    // 初始化点权
    for (int i = 1; i <= n; i++) {
        ct.modify(i, a[i]);
    }

    long long last_ans = 0;
    while (q--) {
        int op;
        long long x, y;
        cin >> op >> x >> y;
        x ^= last_ans;
        y ^= last_ans;

        if (op == 0) {
            // 查询距离x不超过y的点权和
            last_ans = ct.query(x, y);
            cout << last_ans << "\n";
        } else {
            // 修改点x的权值为y
            long long delta = y - a[x];
            a[x] = y;
            ct.modify(x, delta);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
