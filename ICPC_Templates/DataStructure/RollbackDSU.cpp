/**
 * 可撤销并查集 (Rollback DSU)
 * 功能：支持撤销操作的并查集
 * 应用：维护动态图的连通性
 */
#include <bits/stdc++.h>
using namespace std;

struct RollbackDSU {
    vector<int> f, siz;
    stack<pair<int, int>> history;

    RollbackDSU(int n) {
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        siz.assign(n, 1);
    }

    int find(int x) {
        while (x != f[x]) {
            x = f[x];
        }
        return x;
    }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            history.push({-1, -1});
            return false;
        }
        if (siz[x] < siz[y]) {
            swap(x, y);
        }
        history.push({x, y});
        f[y] = x;
        siz[x] += siz[y];
        return true;
    }

    void rollback() {
        auto [x, y] = history.top();
        history.pop();
        if (x == -1) return;
        f[y] = y;
        siz[x] -= siz[y];
    }

    int snapshot() { return history.size(); }

    void rollbackTo(int snap) {
        while (history.size() > snap) {
            rollback();
        }
    }
};
