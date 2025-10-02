// Sum over Subsets DP (SOS DP / 高维前缀和)
// 功能：处理子集和相关的DP问题
// 复杂度：O(n * 2^n)
// 应用：子集枚举、状压DP优化

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 基础SOS DP：计算所有子集的和
// dp[mask] = sum of a[submask] for all submask of mask
template <typename T>
vector<T> sosdp(const vector<T>& a) {
    int n = __builtin_ctz(a.size());  // log2(size)
    vector<T> dp = a;

    for (int i = 0; i < n; i++) {
        for (int mask = 0; mask < (1 << n); mask++) {
            if (mask & (1 << i)) {
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }

    return dp;
}

// 高维前缀和（另一种实现）
template <typename T>
void fwt_or(vector<T>& a, bool inv = false) {
    int n = a.size();
    for (int len = 2, mid = 1; len <= n; mid = len, len <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < mid; j++) {
                if (!inv) {
                    a[i + j + mid] += a[i + j];
                } else {
                    a[i + j + mid] -= a[i + j];
                }
            }
        }
    }
}

// 超集版本：dp[mask] = sum of a[supermask] for all supermask of mask
template <typename T>
vector<T> sosdp_super(const vector<T>& a) {
    int n = __builtin_ctz(a.size());
    vector<T> dp = a;

    for (int i = 0; i < n; i++) {
        for (int mask = (1 << n) - 1; mask >= 0; mask--) {
            if (!(mask & (1 << i))) {
                dp[mask] += dp[mask | (1 << i)];
            }
        }
    }

    return dp;
}

// 应用1：求所有子集的OR值
// 对于每个mask，统计有多少对(i,j)使得 a[i]|a[j] = mask
void example1() {
    int n;
    cin >> n;
    vector<int> a(n);
    int maxval = 0;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        maxval = max(maxval, a[i]);
    }

    int sz = 1;
    while (sz <= maxval) sz <<= 1;

    vector<ll> cnt(sz, 0);
    for (int x : a) {
        cnt[x]++;
    }

    // SOS DP
    auto dp = sosdp(cnt);

    // dp[mask]现在包含了所有子集的个数
    vector<ll> ans(sz, 0);
    for (int mask = 0; mask < sz; mask++) {
        ans[mask] = dp[mask] * dp[mask];  // 可以配对的数量
    }

    // 输出结果
    for (int i = 0; i < sz; i++) {
        if (ans[i] > 0) {
            cout << i << ": " << ans[i] << "\n";
        }
    }
}

// 应用2：子集卷积
// c[mask] = sum over all (s1, s2) where s1 & s2 = 0 and s1 | s2 = mask of a[s1]
// * b[s2]
template <typename T>
vector<T> subset_convolution(const vector<T>& a, const vector<T>& b) {
    int n = __builtin_ctz(a.size());
    int sz = 1 << n;

    // 按popcount分层
    vector<vector<T>> fa(n + 1, vector<T>(sz, 0));
    vector<vector<T>> fb(n + 1, vector<T>(sz, 0));

    for (int mask = 0; mask < sz; mask++) {
        fa[__builtin_popcount(mask)][mask] = a[mask];
        fb[__builtin_popcount(mask)][mask] = b[mask];
    }

    // 对每一层做SOS DP
    for (int i = 0; i <= n; i++) {
        fa[i] = sosdp(fa[i]);
        fb[i] = sosdp(fb[i]);
    }

    // 卷积
    vector<vector<T>> fc(n + 1, vector<T>(sz, 0));
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= i; j++) {
            for (int mask = 0; mask < sz; mask++) {
                fc[i][mask] += fa[j][mask] * fb[i - j][mask];
            }
        }
    }

    // 逆向SOS DP
    for (int i = 0; i <= n; i++) {
        // 逆向：减去子集的贡献
        for (int bit = 0; bit < n; bit++) {
            for (int mask = 0; mask < sz; mask++) {
                if (mask & (1 << bit)) {
                    fc[i][mask] -= fc[i][mask ^ (1 << bit)];
                }
            }
        }
    }

    vector<T> c(sz);
    for (int mask = 0; mask < sz; mask++) {
        c[mask] = fc[__builtin_popcount(mask)][mask];
    }

    return c;
}

// 应用3：最大独立集问题（状压DP优化）
// 给定图的邻接矩阵，求最大独立集的大小
int max_independent_set(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> graph(n, 0);

    // 预处理每个点的邻接关系
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adj[i][j]) {
                graph[i] |= (1 << j);
            }
        }
    }

    int full = (1 << n) - 1;
    vector<int> dp(1 << n, 0);
    dp[0] = 1;

    for (int mask = 0; mask < (1 << n); mask++) {
        if (!dp[mask]) continue;

        // 找到第一个未选择的点
        int first = -1;
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) {
                first = i;
                break;
            }
        }

        if (first == -1) continue;

        // 不选first
        dp[mask | (1 << first)] = max(dp[mask | (1 << first)], dp[mask]);

        // 选first（排除其邻居）
        int next = mask | (1 << first) | (graph[first] & ~mask);
        dp[next] = max(dp[next], dp[mask] + 1);
    }

    return *max_element(dp.begin(), dp.end());
}

// 应用4：最小XOR基
// 给定n个数，求最小的XOR线性基
void min_xor_basis() {
    int n;
    cin >> n;

    const int MAXLOG = 30;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // 统计每个数的出现次数
    map<int, int> cnt;
    for (int x : a) {
        cnt[x]++;
    }

    int sz = 1 << MAXLOG;
    vector<ll> dp(sz, 0);

    for (auto [val, count] : cnt) {
        dp[val] = count;
    }

    // SOS DP找到每个mask的所有子集数量
    auto res = sosdp(dp);

    // 判断是否可以表示
    for (int mask = 0; mask < sz; mask++) {
        if (res[mask] > 0) {
            cout << mask << " ";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    example1();

    return 0;
}
