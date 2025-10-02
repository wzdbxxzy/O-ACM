/**
 * 常系数齐次线性递推
 * BM算法 + 矩阵快速幂
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Berlekamp-Massey算法：求最短线性递推式
template <typename T>
vector<T> BM(const vector<T>& s) {
    vector<T> C(1, 1), B(1, 1);
    int L = 0, m = 1;
    T b = 1;
    for (int n = 0; n < s.size(); n++) {
        T d = 0;
        for (int i = 0; i <= L; i++) d += C[i] * s[n - i];
        if (d == 0) {
            m++;
        } else if (2 * L <= n) {
            auto T = C;
            T c = d / b;
            while (C.size() < B.size() + m) C.push_back(0);
            for (int i = 0; i < B.size(); i++) C[i + m] -= c * B[i];
            L = n + 1 - L;
            B = T;
            b = d;
            m = 1;
        } else {
            T c = d / b;
            while (C.size() < B.size() + m) C.push_back(0);
            for (int i = 0; i < B.size(); i++) C[i + m] -= c * B[i];
            m++;
        }
    }
    return C;
}

// 使用递推式计算第n项
template <typename T>
T linearRecurrence(const vector<T>& init, const vector<T>& trans, ll n) {
    int k = trans.size();
    if (n < k) return init[n];

    // dp[i] = sum(trans[j] * dp[i-k+j])
    vector<T> dp = init;

    // 矩阵快速幂优化
    auto mul = [&](const vector<T>& a, const vector<T>& b) {
        vector<T> c(k * 2 - 1);
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                c[i + j] += a[i] * b[j];
            }
        }
        for (int i = k * 2 - 2; i >= k; i--) {
            for (int j = 0; j < k; j++) {
                c[i - k + j] += c[i] * trans[j];
            }
        }
        c.resize(k);
        return c;
    };

    vector<T> res(k);
    res[0] = 1;
    vector<T> base(k);
    base[1] = 1;

    while (n) {
        if (n & 1) res = mul(res, base);
        base = mul(base, base);
        n >>= 1;
    }

    T ans = 0;
    for (int i = 0; i < k; i++) {
        ans += res[i] * init[i];
    }
    return ans;
}
