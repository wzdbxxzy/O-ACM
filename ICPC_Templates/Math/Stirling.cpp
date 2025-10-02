/**
 * 斯特林数 (Stirling Numbers)
 * 第一类、第二类斯特林数
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;

// 第一类斯特林数：n个元素分成k个非空轮换的方案数
// S1(n, k) = S1(n-1, k-1) + (n-1) * S1(n-1, k)
vector<vector<ll>> stirling1(int n) {
    vector<vector<ll>> s(n + 1, vector<ll>(n + 1));
    s[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            s[i][j] = (s[i - 1][j - 1] + (i - 1) * s[i - 1][j]) % MOD;
        }
    }
    return s;
}

// 第二类斯特林数：n个不同元素分成k个非空集合的方案数
// S2(n, k) = S2(n-1, k-1) + k * S2(n-1, k)
vector<vector<ll>> stirling2(int n) {
    vector<vector<ll>> s(n + 1, vector<ll>(n + 1));
    s[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            s[i][j] = (s[i - 1][j - 1] + j * s[i - 1][j]) % MOD;
        }
    }
    return s;
}

// 第二类斯特林数单行（容斥原理）
// S2(n, m) = (1/m!) * sum((-1)^i * C(m,i) * (m-i)^n)
ll stirling2_single(ll n, ll m, ll mod = MOD) {
    if (m > n) return 0;

    auto power = [&](ll a, ll b) {
        ll res = 1;
        for (; b; b >>= 1, a = a * a % mod) {
            if (b & 1) res = res * a % mod;
        }
        return res;
    };

    // 预处理阶乘
    vector<ll> fac(m + 1), inv(m + 1);
    fac[0] = 1;
    for (int i = 1; i <= m; i++) {
        fac[i] = fac[i - 1] * i % mod;
    }
    inv[m] = power(fac[m], mod - 2);
    for (int i = m - 1; i >= 0; i--) {
        inv[i] = inv[i + 1] * (i + 1) % mod;
    }

    ll res = 0;
    for (int i = 0; i <= m; i++) {
        ll tmp = inv[i] * inv[m - i] % mod * power(m - i, n) % mod;
        if (i & 1) {
            res = (res - tmp + mod) % mod;
        } else {
            res = (res + tmp) % mod;
        }
    }

    return res * fac[m] % mod;
}

// 贝尔数：n个元素的划分方案总数
// Bell(n) = sum(S2(n, k)) for k = 0 to n
vector<ll> bellNumbers(int n) {
    auto s2 = stirling2(n);
    vector<ll> bell(n + 1);
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= i; j++) {
            bell[i] = (bell[i] + s2[i][j]) % MOD;
        }
    }
    return bell;
}
