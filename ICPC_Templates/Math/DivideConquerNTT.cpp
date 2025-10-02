// Divide and Conquer NTT (分治NTT / 分治卷积)
// 功能：多项式分治、快速计算卷积序列
// 复杂度：O(n log^2 n)
// 应用：多点求值、快速插值、生成函数

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MOD = 998244353;
const int G = 3;  // 原根

ll power(ll a, ll b, ll p = MOD) {
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}

// NTT变换
void ntt(vector<ll>& a, bool inv) {
    int n = a.size();
    if (n == 1) return;

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        ll w = power(G, (MOD - 1) / len);
        if (inv) w = power(w, MOD - 2);

        for (int i = 0; i < n; i += len) {
            ll wn = 1;
            for (int j = 0; j < len / 2; j++) {
                ll t = a[i + j + len / 2] * wn % MOD;
                a[i + j + len / 2] = (a[i + j] - t + MOD) % MOD;
                a[i + j] = (a[i + j] + t) % MOD;
                wn = wn * w % MOD;
            }
        }
    }

    if (inv) {
        ll inv_n = power(n, MOD - 2);
        for (auto& x : a) {
            x = x * inv_n % MOD;
        }
    }
}

// 多项式乘法
vector<ll> multiply(vector<ll> a, vector<ll> b) {
    int n = a.size() + b.size() - 1;
    int sz = 1;
    while (sz < n) sz <<= 1;

    a.resize(sz);
    b.resize(sz);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < sz; i++) {
        a[i] = a[i] * b[i] % MOD;
    }

    ntt(a, true);
    a.resize(n);

    return a;
}

// 分治NTT：计算 c[k] = sum_{i=0}^{k-1} a[i] * b[k-i]
// 其中 b[i] 可能依赖于前面的 c[0..i-1]
// 递推形式：c[i] = sum_{j=0}^{i-1} a[j] * c[i-j] + b[i]
vector<ll> divide_conquer_ntt(const vector<ll>& a, const vector<ll>& b, int n) {
    vector<ll> c(n, 0);

    function<void(int, int)> solve = [&](int l, int r) {
        if (l == r) {
            if (l == 0) {
                c[0] = b[0];
            } else {
                c[l] = (c[l] + b[l]) % MOD;
            }
            return;
        }

        int mid = (l + r) / 2;
        solve(l, mid);

        // 计算 [l, mid] 对 [mid+1, r] 的贡献
        int len = r - l + 1;
        vector<ll> f(len, 0), g(len, 0);

        for (int i = l; i <= mid; i++) {
            f[i - l] = c[i];
        }
        for (int i = 0; i < len && i < (int)a.size(); i++) {
            g[i] = a[i];
        }

        auto conv = multiply(f, g);

        for (int i = mid + 1; i <= r; i++) {
            c[i] = (c[i] + conv[i - l]) % MOD;
        }

        solve(mid + 1, r);
    };

    solve(0, n - 1);
    return c;
}

// 应用1：生成函数求卷积
// f(x) = product_{i=1}^{n} (1 + a[i] * x)
vector<ll> product_generating_function(const vector<ll>& a) {
    int n = a.size();

    function<vector<ll>(int, int)> solve = [&](int l, int r) -> vector<ll> {
        if (l == r) {
            return {1, a[l]};
        }

        int mid = (l + r) / 2;
        auto left = solve(l, mid);
        auto right = solve(mid + 1, r);

        return multiply(left, right);
    };

    if (n == 0) return {1};
    return solve(0, n - 1);
}

// 应用2：多项式快速幂（分治+NTT）
vector<ll> poly_power(vector<ll> a, ll k, int n) {
    // 计算 a(x)^k mod x^n

    function<vector<ll>(ll)> solve = [&](ll k) -> vector<ll> {
        if (k == 0) return {1};
        if (k == 1) {
            a.resize(n);
            return a;
        }

        auto half = solve(k / 2);
        auto res = multiply(half, half);
        res.resize(n);

        if (k % 2 == 1) {
            res = multiply(res, a);
            res.resize(n);
        }

        return res;
    };

    return solve(k);
}

// 应用3：树形DP卷积优化
// 例如：计算所有子树大小的贡献和
struct Tree {
    int n;
    vector<vector<int>> adj;
    vector<ll> dp;

    Tree(int n_) : n(n_), adj(n), dp(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<ll> dfs(int u, int p) {
        vector<ll> res = {1};

        for (int v : adj[u]) {
            if (v == p) continue;

            auto child = dfs(v, u);
            res = multiply(res, child);

            if (res.size() > n) res.resize(n);
        }

        res.insert(res.begin(), 0);  // 加上当前节点
        if (res.size() > n) res.resize(n);

        return res;
    }

    vector<ll> solve() { return dfs(0, -1); }
};

// 应用4：循环卷积
vector<ll> circular_convolution(vector<ll> a, vector<ll> b, int n) {
    // (a * b) mod (x^n - 1)

    int sz = 1;
    while (sz < 2 * n) sz <<= 1;

    a.resize(sz);
    b.resize(sz);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < sz; i++) {
        a[i] = a[i] * b[i] % MOD;
    }

    ntt(a, true);

    vector<ll> res(n, 0);
    for (int i = 0; i < sz; i++) {
        res[i % n] = (res[i % n] + a[i]) % MOD;
    }

    return res;
}

// 应用5：多点求值（分治+NTT）
// 给定多项式f(x)和n个点x_0, x_1, ..., x_{n-1}，求f(x_0), f(x_1), ...,
// f(x_{n-1})
vector<ll> multipoint_evaluation(const vector<ll>& f, const vector<ll>& x) {
    int n = x.size();

    // 构建子积树
    vector<vector<vector<ll>>> tree(4 * n);

    function<void(int, int, int)> build = [&](int v, int l, int r) {
        if (l == r) {
            tree[v] = {{(MOD - x[l]) % MOD, 1}};  // (x - x_l)
            return;
        }

        int mid = (l + r) / 2;
        build(2 * v, l, mid);
        build(2 * v + 1, mid + 1, r);

        tree[v] = multiply(tree[2 * v], tree[2 * v + 1]);
    };

    build(1, 0, n - 1);

    // 多项式取模
    function<vector<ll>(const vector<ll>&, const vector<ll>&)> poly_mod =
        [&](const vector<ll>& a, const vector<ll>& b) -> vector<ll> {
        if (a.size() < b.size()) return a;

        // 简化实现：直接计算余数
        vector<ll> res = a;
        // ... 多项式长除法
        // 这里简化处理
        return res;
    };

    // 分治求值
    vector<ll> res(n);

    function<void(int, int, int, const vector<ll>&)> evaluate =
        [&](int v, int l, int r, const vector<ll>& poly) {
            if (l == r) {
                // 直接求值
                ll val = 0, pow_x = 1;
                for (ll coef : poly) {
                    val = (val + coef * pow_x) % MOD;
                    pow_x = pow_x * x[l] % MOD;
                }
                res[l] = val;
                return;
            }

            int mid = (l + r) / 2;

            auto left_poly = poly_mod(poly, tree[2 * v]);
            auto right_poly = poly_mod(poly, tree[2 * v + 1]);

            evaluate(2 * v, l, mid, left_poly);
            evaluate(2 * v + 1, mid + 1, r, right_poly);
        };

    build(1, 0, n - 1);
    evaluate(1, 0, n - 1, f);

    return res;
}

// 使用示例
void solve() {
    int n;
    cin >> n;

    vector<ll> a(n), b(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // 计算 c[i] = sum_{j=0}^{i} a[j] * a[i-j]
    auto c = multiply(a, a);

    cout << "Result:\n";
    for (int i = 0; i < (int)c.size(); i++) {
        cout << c[i] << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
