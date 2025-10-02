/**
 * 位运算技巧
 * 常用的位运算操作和技巧
 */
#include <bits/stdc++.h>
using namespace std;

// 位运算基础操作
namespace BitOps {
// 判断第k位是否为1
bool getBit(int x, int k) { return (x >> k) & 1; }

// 将第k位设为1
int setBit(int x, int k) { return x | (1 << k); }

// 将第k位设为0
int clearBit(int x, int k) { return x & ~(1 << k); }

// 翻转第k位
int flipBit(int x, int k) { return x ^ (1 << k); }

// 统计1的个数
int popcount(int x) { return __builtin_popcount(x); }

int popcountll(long long x) { return __builtin_popcountll(x); }

// 最低位的1
int lowbit(int x) { return x & -x; }

// 最高位的1的位置（0-based）
int highbit(int x) { return 31 - __builtin_clz(x); }

// 前导零个数
int clz(int x) { return __builtin_clz(x); }

// 后导零个数
int ctz(int x) { return __builtin_ctz(x); }

// 是否是2的幂
bool isPowerOfTwo(int x) { return x > 0 && (x & (x - 1)) == 0; }

// 向上取整到2的幂
int nextPowerOfTwo(int x) {
    if (x <= 1) return 1;
    return 1 << (32 - __builtin_clz(x - 1));
}

// 枚举所有子集
void enumerateSubsets(int mask) {
    for (int s = mask; s; s = (s - 1) & mask) {
        // 处理子集s
    }
}

// 枚举k个1的所有状态
void enumerateKBits(int n, int k) {
    int mask = (1 << k) - 1;
    while (mask < (1 << n)) {
        // 处理mask
        int c = mask & -mask;
        int r = mask + c;
        mask = (((r ^ mask) >> 2) / c) | r;
    }
}
}  // namespace BitOps

// 高精度位运算（大数）
struct BigInt {
    vector<int> bits;

    BigInt(int n = 0) {
        while (n) {
            bits.push_back(n & 1);
            n >>= 1;
        }
    }

    BigInt operator&(const BigInt& other) const {
        BigInt res;
        int n = min(bits.size(), other.bits.size());
        res.bits.resize(n);
        for (int i = 0; i < n; i++) {
            res.bits[i] = bits[i] & other.bits[i];
        }
        return res;
    }

    BigInt operator|(const BigInt& other) const {
        BigInt res;
        int n = max(bits.size(), other.bits.size());
        res.bits.resize(n);
        for (int i = 0; i < n; i++) {
            int a = (i < bits.size()) ? bits[i] : 0;
            int b = (i < other.bits.size()) ? other.bits[i] : 0;
            res.bits[i] = a | b;
        }
        return res;
    }

    BigInt operator^(const BigInt& other) const {
        BigInt res;
        int n = max(bits.size(), other.bits.size());
        res.bits.resize(n);
        for (int i = 0; i < n; i++) {
            int a = (i < bits.size()) ? bits[i] : 0;
            int b = (i < other.bits.size()) ? other.bits[i] : 0;
            res.bits[i] = a ^ b;
        }
        return res;
    }
};

// 快速Walsh变换（FWT）
namespace FWT {
const int MOD = 998244353;

void fwt_or(vector<int>& a, bool inv) {
    int n = a.size();
    for (int len = 2, mid = 1; len <= n; mid = len, len <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < mid; j++) {
                if (!inv) {
                    a[i + j + mid] = (a[i + j + mid] + a[i + j]) % MOD;
                } else {
                    a[i + j + mid] = (a[i + j + mid] - a[i + j] + MOD) % MOD;
                }
            }
        }
    }
}

void fwt_and(vector<int>& a, bool inv) {
    int n = a.size();
    for (int len = 2, mid = 1; len <= n; mid = len, len <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < mid; j++) {
                if (!inv) {
                    a[i + j] = (a[i + j] + a[i + j + mid]) % MOD;
                } else {
                    a[i + j] = (a[i + j] - a[i + j + mid] + MOD) % MOD;
                }
            }
        }
    }
}

void fwt_xor(vector<int>& a, bool inv) {
    int n = a.size();
    int inv2 = (MOD + 1) / 2;
    for (int len = 2, mid = 1; len <= n; mid = len, len <<= 1) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < mid; j++) {
                int x = a[i + j], y = a[i + j + mid];
                if (!inv) {
                    a[i + j] = (x + y) % MOD;
                    a[i + j + mid] = (x - y + MOD) % MOD;
                } else {
                    a[i + j] = 1LL * (x + y) * inv2 % MOD;
                    a[i + j + mid] = 1LL * (x - y + MOD) * inv2 % MOD;
                }
            }
        }
    }
}
}  // namespace FWT
