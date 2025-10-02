/**
 * 字符串哈希
 * 功能：快速字符串匹配、子串比较
 * 复杂度：O(n) 预处理，O(1) 查询
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct StringHash {
    static constexpr ll P = 131;
    static constexpr ll MOD = 1e9 + 7;

    vector<ll> h, p;
    int n;

    StringHash() {}
    StringHash(const string& s) {
        n = s.size();
        h.resize(n + 1);
        p.resize(n + 1);
        p[0] = 1;

        for (int i = 0; i < n; i++) {
            h[i + 1] = (h[i] * P + s[i]) % MOD;
            p[i + 1] = p[i] * P % MOD;
        }
    }

    // 获取区间 [l, r) 的哈希值
    ll get(int l, int r) { return (h[r] - h[l] * p[r - l] % MOD + MOD) % MOD; }

    // 比较两个子串是否相等
    bool equal(int l1, int r1, int l2, int r2) {
        return get(l1, r1) == get(l2, r2);
    }
};

// 双哈希（更安全）
struct DoubleHash {
    static constexpr ll P1 = 131, P2 = 13331;
    static constexpr ll MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;

    vector<ll> h1, h2, p1, p2;
    int n;

    DoubleHash() {}
    DoubleHash(const string& s) {
        n = s.size();
        h1.resize(n + 1);
        h2.resize(n + 1);
        p1.resize(n + 1);
        p2.resize(n + 1);
        p1[0] = p2[0] = 1;

        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * P1 + s[i]) % MOD1;
            h2[i + 1] = (h2[i] * P2 + s[i]) % MOD2;
            p1[i + 1] = p1[i] * P1 % MOD1;
            p2[i + 1] = p2[i] * P2 % MOD2;
        }
    }

    pair<ll, ll> get(int l, int r) {
        ll hash1 = (h1[r] - h1[l] * p1[r - l] % MOD1 + MOD1) % MOD1;
        ll hash2 = (h2[r] - h2[l] * p2[r - l] % MOD2 + MOD2) % MOD2;
        return {hash1, hash2};
    }

    bool equal(int l1, int r1, int l2, int r2) {
        return get(l1, r1) == get(l2, r2);
    }
};
