/**
 * 素数相关算法
 * 欧拉筛、素性测试、分解质因数
 */
#include <bits/stdc++.h>
using namespace std;

// 欧拉筛（线性筛）
// 复杂度：O(n)
vector<int> minp, primes;

void sieve(int n) {
    minp.assign(n + 1, 0);
    primes.clear();

    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            primes.push_back(i);
        }

        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            minp[i * p] = p;
            if (p == minp[i]) {
                break;
            }
        }
    }
}

bool isprime(int n) { return minp[n] == n; }

// 试除法分解质因数
// 复杂度：O(sqrt(n))
vector<pair<int, int>> factorize(int n) {
    vector<pair<int, int>> res;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                cnt++;
            }
            res.emplace_back(i, cnt);
        }
    }
    if (n > 1) {
        res.emplace_back(n, 1);
    }
    return res;
}

// Miller-Rabin 素性测试
// 复杂度：O(k log^3 n)，k为测试次数
using u64 = unsigned long long;

u64 mulmod(u64 a, u64 b, u64 m) { return __int128(a) * b % m; }

u64 powmod(u64 a, u64 b, u64 m) {
    u64 res = 1;
    for (; b; b >>= 1, a = mulmod(a, a, m)) {
        if (b & 1) {
            res = mulmod(res, a, m);
        }
    }
    return res;
}

bool miller_rabin(u64 n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    u64 d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    for (u64 a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n == a) return true;
        u64 x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (int i = 0; i < r - 1; i++) {
            x = mulmod(x, x, n);
            if (x == n - 1) {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}
