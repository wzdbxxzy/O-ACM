/**
 * 并查集 (Disjoint Set Union)
 * 功能：维护不相交集合，支持合并和查询
 * 复杂度：O(α(n)) 均摊
 */
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> f, siz;

    DSU() {}
    DSU(int n) { init(n); }

    void init(int n) {
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        siz.assign(n, 1);
    }

    int find(int x) {
        while (x != f[x]) {
            x = f[x] = f[f[x]];
        }
        return x;
    }

    bool same(int x, int y) { return find(x) == find(y); }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        siz[x] += siz[y];
        f[y] = x;
        return true;
    }

    int size(int x) { return siz[find(x)]; }
};
