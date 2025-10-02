/**
 * 扩展欧几里得算法 (Extended GCD)
 * 功能：求解 ax + by = gcd(a, b)
 * 应用：求逆元、线性同余方程
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

// 求 a 在模 m 下的逆元
// 前提：gcd(a, m) = 1
ll inv(ll a, ll m) {
    ll x, y;
    ll d = exgcd(a, m, x, y);
    if (d != 1) return -1;  // 无逆元
    return (x % m + m) % m;
}

// 求解线性同余方程 ax ≡ b (mod m)
// 返回最小非负整数解，无解返回-1
ll linear_congruence(ll a, ll b, ll m) {
    ll x, y;
    ll d = exgcd(a, m, x, y);
    if (b % d != 0) return -1;
    x = x * (b / d) % m;
    return (x % m + m) % m;
}

// 中国剩余定理 (CRT)
// 求解 x ≡ a[i] (mod m[i])
// 前提：m[i] 两两互质
ll crt(const vector<ll>& a, const vector<ll>& m) {
    ll M = 1, ans = 0;
    int n = a.size();
    for (int i = 0; i < n; i++) {
        M *= m[i];
    }
    for (int i = 0; i < n; i++) {
        ll Mi = M / m[i];
        ll x, y;
        exgcd(Mi, m[i], x, y);
        ans = (ans + a[i] * Mi * x) % M;
    }
    return (ans % M + M) % M;
}
