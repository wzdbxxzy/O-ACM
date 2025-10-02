/**
 * 动态规划优化技巧
 * 斜率优化、单调队列优化等
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 1. 斜率优化DP（凸壳优化）
// 适用于：dp[i] = min(dp[j] + cost(j, i)) 且cost满足四边形不等式
namespace SlopeOptimization {
struct Line {
    ll k, b;
    ll eval(ll x) const { return k * x + b; }
};

bool bad(Line l1, Line l2, Line l3) {
    // 判断l2是否被l1和l3夹在中间（不优）
    return (__int128)(l3.b - l1.b) * (l1.k - l2.k) <=
           (__int128)(l2.b - l1.b) * (l1.k - l3.k);
}

vector<ll> slopeDP(int n, const vector<ll>& a) {
    vector<ll> dp(n + 1);
    deque<Line> hull;

    hull.push_back({0, 0});

    for (int i = 1; i <= n; i++) {
        // 维护凸壳
        while (hull.size() >= 2 && hull[0].eval(a[i]) >= hull[1].eval(a[i])) {
            hull.pop_front();
        }

        dp[i] = hull[0].eval(a[i]);

        Line newLine = {-a[i], dp[i]};

        while (hull.size() >= 2 &&
               bad(hull[hull.size() - 2], hull.back(), newLine)) {
            hull.pop_back();
        }

        hull.push_back(newLine);
    }

    return dp;
}
}  // namespace SlopeOptimization

// 2. 单调队列优化DP
// 适用于：dp[i] = min(dp[j] + cost[i]) (j in [l, r])
namespace MonotoneQueueOptimization {
vector<ll> monotonicQueueDP(int n, const vector<ll>& cost, int k) {
    vector<ll> dp(n + 1, 1e18);
    dp[0] = 0;
    deque<int> q;

    for (int i = 1; i <= n; i++) {
        // 移除超出窗口的元素
        while (!q.empty() && q.front() < i - k) {
            q.pop_front();
        }

        // 维护单调性
        while (!q.empty() && dp[q.back()] >= dp[i - 1]) {
            q.pop_back();
        }

        q.push_back(i - 1);

        if (!q.empty()) {
            dp[i] = dp[q.front()] + cost[i];
        }
    }

    return dp;
}
}  // namespace MonotoneQueueOptimization

// 3. 四边形不等式优化
// 适用于：dp[i][j] = min(dp[i][k] + dp[k+1][j] + w[i][j])
namespace QuadrangleInequality {
vector<vector<ll>> quadrangleDP(int n, const vector<vector<ll>>& w) {
    vector<vector<ll>> dp(n, vector<ll>(n, 1e18));
    vector<vector<int>> split(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        dp[i][i] = 0;
        split[i][i] = i;
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            int l = split[i][j - 1];
            int r = (i + 1 < n) ? split[i + 1][j] : j;

            for (int k = l; k <= min(r, j - 1); k++) {
                ll val = dp[i][k] + dp[k + 1][j] + w[i][j];
                if (val < dp[i][j]) {
                    dp[i][j] = val;
                    split[i][j] = k;
                }
            }
        }
    }

    return dp;
}
}  // namespace QuadrangleInequality

// 4. 决策单调性优化
// 分治优化
namespace DivideAndConquer {
ll cost(int l, int r);  // 自定义代价函数

void solve(int l, int r, int optl, int optr, vector<ll>& dp,
           const vector<ll>& pre) {
    if (l > r) return;

    int mid = (l + r) / 2;
    ll best = 1e18;
    int bestk = -1;

    for (int k = optl; k <= min(optr, mid - 1); k++) {
        ll val = pre[k] + cost(k + 1, mid);
        if (val < best) {
            best = val;
            bestk = k;
        }
    }

    dp[mid] = best;

    solve(l, mid - 1, optl, bestk, dp, pre);
    solve(mid + 1, r, bestk, optr, dp, pre);
}
}  // namespace DivideAndConquer
