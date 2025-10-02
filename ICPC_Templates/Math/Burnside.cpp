/**
 * Burnside引理 / Polya定理
 * 功能：计数本质不同的染色方案
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Burnside引理：本质不同方案数 = (1/|G|) * sum(|X^g|)
// 其中|X^g|表示在置换g下不动点的个数

// 求gcd
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }

// 快速幂
ll power(ll a, ll b, ll mod) {
    ll res = 1;
    for (; b; b >>= 1, a = a * a % mod) {
        if (b & 1) res = res * a % mod;
    }
    return res;
}

// 项链染色问题：n个珠子，m种颜色，旋转等价
// 答案 = (1/n) * sum(m^gcd(n,k)) for k=0 to n-1
ll necklace(ll n, ll m, ll mod) {
    ll res = 0;
    for (ll k = 0; k < n; k++) {
        res = (res + power(m, gcd(n, k), mod)) % mod;
    }
    return res * power(n, mod - 2, mod) % mod;
}

// 手镯染色问题：n个珠子，m种颜色，旋转+翻转等价
// 需要分n为奇数和偶数讨论
ll bracelet(ll n, ll m, ll mod) {
    ll res = 0;
    // 旋转
    for (ll k = 0; k < n; k++) {
        res = (res + power(m, gcd(n, k), mod)) % mod;
    }
    // 翻转
    if (n % 2 == 1) {
        // n为奇数：n种翻转，每种有(n+1)/2个不动点
        res = (res + n * power(m, (n + 1) / 2, mod)) % mod;
    } else {
        // n为偶数：n/2种穿过顶点的翻转，n/2种穿过边中点的翻转
        res = (res + (n / 2) * power(m, n / 2 + 1, mod)) % mod;
        res = (res + (n / 2) * power(m, n / 2, mod)) % mod;
    }
    return res * power(2 * n, mod - 2, mod) % mod;
}

// Polya定理的一般形式
// 计算在群G作用下，使用m种颜色对n个位置染色的不同方案数
struct Polya {
    ll n, m, mod;

    Polya(ll n, ll m, ll mod) : n(n), m(m), mod(mod) {}

    // 计算置换的循环节数
    ll countCycles(const vector<int>& perm) {
        int n = perm.size();
        vector<bool> vis(n);
        int cycles = 0;
        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                cycles++;
                int j = i;
                while (!vis[j]) {
                    vis[j] = true;
                    j = perm[j];
                }
            }
        }
        return cycles;
    }

    // 给定群的所有置换，计算染色方案数
    ll solve(const vector<vector<int>>& group) {
        ll res = 0;
        for (auto& perm : group) {
            ll cycles = countCycles(perm);
            res = (res + power(m, cycles, mod)) % mod;
        }
        return res * power(group.size(), mod - 2, mod) % mod;
    }
};
