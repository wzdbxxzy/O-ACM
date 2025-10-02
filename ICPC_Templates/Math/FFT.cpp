/**
 * 快速傅里叶变换 (Fast Fourier Transform)
 * 功能：多项式乘法（实数）
 * 复杂度：O(n log n)
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using cd = complex<double>;
const double PI = acos(-1);

// FFT (complex, iterative)
void fft(vector<cd>& a, bool invert) {
    int n = a.size();
    // bit-reversed permutation
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    // iterative FFT
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (cd& x : a) x /= n;
    }
}

// multiply two integer sequences
vector<ll> multiply_fft(const vector<ll>& a, const vector<ll>& b) {
    if (a.empty() || b.empty()) return {};
    int n = 1;
    while (n < (int)a.size() + (int)b.size()) n <<= 1;
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; ++i) fa[i] *= fb[i];
    fft(fa, true);
    vector<ll> res(n);
    for (int i = 0; i < n; ++i) res[i] = (ll)llround(fa[i].real());
    while (!res.empty() && res.back() == 0) res.pop_back();
    return res;
}
