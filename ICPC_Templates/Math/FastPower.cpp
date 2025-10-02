/**
 * 快速幂 (Fast Power)
 * 功能：快速计算 a^b % p
 * 复杂度：O(log b)
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 普通版本
ll power(ll a, ll b, ll p) {
    ll res = 1;
    for (; b; b /= 2, a = a * a % p) {
        if (b % 2) {
            res = res * a % p;
        }
    }
    return res;
}

// 手写乘法版本（防止爆long long）
ll mul(ll a, ll b, ll p) {
    ll c = a * b - ll(1.0L * a * b / p) * p;
    c %= p;
    if (c < 0) {
        c += p;
    }
    return c;
}

ll power_safe(ll a, ll b, ll p) {
    ll res = 1;
    for (; b; b /= 2, a = mul(a, a, p)) {
        if (b % 2) {
            res = mul(res, a, p);
        }
    }
    return res;
}
