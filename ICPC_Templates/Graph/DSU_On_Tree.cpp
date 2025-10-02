// DSU on Tree (树上启发式合并)
// 功能：高效处理树上子树的统计问题
// 复杂度：O(n log n)
// 应用：统计子树内满足条件的节点数等

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5e5 + 10;

vector<int> g[MAXN];
int sz[MAXN], son[MAXN];  // sz: 子树大小, son: 重儿子
int cnt[MAXN];            // 统计数组（根据具体问题调整）
bool big[MAXN];           // 标记是否保留重儿子的信息
int ans[MAXN];

// 预处理：计算子树大小和重儿子
void dfs_sz(int u, int fa) {
    sz[u] = 1;
    for (int v : g[u]) {
        if (v == fa) continue;
        dfs_sz(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[son[u]]) {
            son[u] = v;
        }
    }
}

// 添加/删除子树的贡献
// sign = 1: 添加, sign = -1: 删除
void add(int u, int fa, int sign) {
    // 根据具体问题修改统计逻辑
    cnt[u] += sign;  // 示例：统计节点出现次数

    for (int v : g[u]) {
        if (v == fa || big[v]) continue;
        add(v, u, sign);
    }
}

// DSU on Tree 主体
void dfs(int u, int fa, bool keep) {
    // 先处理轻儿子
    for (int v : g[u]) {
        if (v == fa || v == son[u]) continue;
        dfs(v, u, false);  // 轻儿子不保留信息
    }

    // 处理重儿子
    if (son[u]) {
        dfs(son[u], u, true);  // 重儿子保留信息
        big[son[u]] = true;
    }

    // 添加轻儿子和当前节点的贡献
    add(u, fa, 1);

    // 统计答案（根据具体问题修改）
    ans[u] = cnt[u];  // 示例

    // 取消重儿子标记
    if (son[u]) big[son[u]] = false;

    // 如果不保留，清空当前子树的贡献
    if (!keep) {
        add(u, fa, -1);
    }
}

// 使用示例
void solve() {
    int n;
    cin >> n;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    dfs_sz(1, 0);
    dfs(1, 0, false);

    // 输出答案
    for (int i = 1; i <= n; i++) {
        cout << ans[i] << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
