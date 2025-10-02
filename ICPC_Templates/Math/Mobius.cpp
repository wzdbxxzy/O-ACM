/**
 * 莫比乌斯函数 (Mobius Function)
 * 功能：莫比乌斯反演相关
 * 复杂度：O(n) 预处理
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> minp, primes, phi, mu;
vector<ll> sphi;

void sieve(int n) {
    minp.assign(n + 1, 0);
    phi.assign(n + 1, 0);
    sphi.assign(n + 1, 0);
    mu.assign(n + 1, 0);
    primes.clear();
    phi[1] = 1;
    mu[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            phi[i] = i - 1;
            mu[i] = -1;
            primes.push_back(i);
        }

        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            minp[i * p] = p;
            if (p == minp[i]) {
                phi[i * p] = phi[i] * p;
                break;
            }
            phi[i * p] = phi[i] * (p - 1);
            mu[i * p] = -mu[i];
        }
    }

    // 前缀和
    for (int i = 1; i <= n; i++) {
        sphi[i] = sphi[i - 1] + phi[i];
        mu[i] += mu[i - 1];
    }
}

// 杜教筛求莫比乌斯函数前缀和
unordered_map<int, ll> fMu;

ll sumMu(int n) {
    if (n < mu.size()) {
        return mu[n];
    }
    if (fMu.count(n)) {
        return fMu[n];
    }
    ll ans = 1;
    for (int l = 2, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans -= (r - l + 1) * sumMu(n / l);
    }
    return fMu[n] = ans;
}

// 杜教筛求欧拉函数前缀和
unordered_map<int, ll> fPhi;

ll sumPhi(int n) {
    if (n < sphi.size()) {
        return sphi[n];
    }
    if (fPhi.count(n)) {
        return fPhi[n];
    }
    ll ans = 1LL * n * (n + 1) / 2;
    for (int l = 2, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans -= (r - l + 1) * sumPhi(n / l);
    }
    return fPhi[n] = ans;
}
