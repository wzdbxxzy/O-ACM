/**
 * 组合数学
 * 阶乘、组合数、Lucas定理
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 组合数预处理（小范围）
struct Comb {
    vector<ll> fac, inv;
    ll mod;

    Comb(int n, ll mod = 1e9 + 7) : mod(mod) {
        fac.resize(n + 1);
        inv.resize(n + 1);
        fac[0] = 1;
        for (int i = 1; i <= n; i++) {
            fac[i] = fac[i - 1] * i % mod;
        }
        inv[n] = power(fac[n], mod - 2, mod);
        for (int i = n - 1; i >= 0; i--) {
            inv[i] = inv[i + 1] * (i + 1) % mod;
        }
    }

    ll power(ll a, ll b, ll p) {
        ll res = 1;
        for (; b; b /= 2, a = a * a % p) {
            if (b % 2) {
                res = res * a % p;
            }
        }
        return res;
    }

    ll C(int n, int m) {
        if (m < 0 || m > n) return 0;
        return fac[n] * inv[m] % mod * inv[n - m] % mod;
    }

    ll A(int n, int m) {
        if (m < 0 || m > n) return 0;
        return fac[n] * inv[n - m] % mod;
    }
};

// Lucas定理（p为质数）
ll lucas(ll n, ll m, ll p) {
    if (m == 0) return 1;

    auto power = [&](ll a, ll b) {
        ll res = 1;
        for (; b; b /= 2, a = a * a % p) {
            if (b % 2) {
                res = res * a % p;
            }
        }
        return res;
    };

    auto C = [&](ll n, ll m) -> ll {
        if (m > n) return 0;
        ll res = 1;
        for (int i = 1; i <= m; i++) {
            res = res * (n - i + 1) % p;
            res = res * power(i, p - 2) % p;
        }
        return res;
    };

    return C(n % p, m % p) * lucas(n / p, m / p, p) % p;
}
