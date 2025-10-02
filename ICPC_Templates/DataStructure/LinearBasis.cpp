/**
 * 线性基 (Linear Basis)
 * 功能：维护异或空间的一组基
 * 应用：最大异或和，异或第k小等
 */
#include <bits/stdc++.h>
using namespace std;

struct Basis {
    int a[20]{};
    int t[20]{};

    Basis() { fill(t, t + 20, -1); }

    void add(int x, int y = 1e9) {
        for (int i = 0; i < 20; i++) {
            if (x >> i & 1) {
                if (y > t[i]) {
                    swap(a[i], x);
                    swap(t[i], y);
                }
                x ^= a[i];
            }
        }
    }

    bool query(int x, int y = 0) {
        for (int i = 0; i < 20; i++) {
            if ((x >> i & 1) && t[i] >= y) {
                x ^= a[i];
            }
        }
        return x == 0;
    }

    int queryMax() {
        int res = 0;
        for (int i = 19; i >= 0; i--) {
            res = max(res, res ^ a[i]);
        }
        return res;
    }
};
