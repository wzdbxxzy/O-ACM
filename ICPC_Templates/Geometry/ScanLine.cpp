/**
 * 扫描线算法
 * 功能：矩形面积并、周长并等
 * 复杂度：O(n log n)
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 矩形面积并
struct Rectangle {
    ll x1, y1, x2, y2;
};

ll rectangleUnion(vector<Rectangle> rects) {
    vector<tuple<ll, ll, ll, int>> events;  // (x, y1, y2, type)

    for (auto [x1, y1, x2, y2] : rects) {
        events.push_back({x1, y1, y2, 1});   // 左边界
        events.push_back({x2, y1, y2, -1});  // 右边界
    }

    sort(events.begin(), events.end());

    ll area = 0;
    ll lastX = 0;
    map<ll, int> cnt;  // y坐标的计数

    auto calcLen = [&]() {
        ll len = 0;
        ll last = -1e18;
        for (auto [y, c] : cnt) {
            if (c > 0) {
                if (last < -1e17) {
                    last = y;
                }
            } else {
                if (last > -1e17) {
                    len += y - last;
                    last = -1e18;
                }
            }
        }
        return len;
    };

    for (auto [x, y1, y2, type] : events) {
        if (!cnt.empty()) {
            ll len = calcLen();
            area += len * (x - lastX);
        }

        cnt[y1] += type;
        cnt[y2] -= type;
        if (cnt[y1] == 0) cnt.erase(y1);
        if (cnt[y2] == 0) cnt.erase(y2);

        lastX = x;
    }

    return area;
}

// 使用线段树优化的版本
struct ScanLineSegTree {
    struct Node {
        int cnt = 0;
        ll len = 0;
    };

    int n;
    vector<Node> tree;
    vector<ll> ys;

    ScanLineSegTree(vector<ll> y) : ys(y) {
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        n = ys.size();
        tree.resize(4 * n);
    }

    void push_up(int p, int l, int r) {
        if (tree[p].cnt > 0) {
            tree[p].len = ys[r] - ys[l];
        } else if (r - l == 1) {
            tree[p].len = 0;
        } else {
            tree[p].len = tree[2 * p].len + tree[2 * p + 1].len;
        }
    }

    void update(int p, int l, int r, int x, int y, int v) {
        if (x >= y) return;
        if (l >= y || r <= x) return;
        if (l >= x && r <= y) {
            tree[p].cnt += v;
            push_up(p, l, r);
            return;
        }
        int m = (l + r) / 2;
        update(2 * p, l, m, x, y, v);
        update(2 * p + 1, m, r, x, y, v);
        push_up(p, l, r);
    }

    void update(ll y1, ll y2, int v) {
        int i1 = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        int i2 = lower_bound(ys.begin(), ys.end(), y2) - ys.begin();
        update(1, 0, n - 1, i1, i2, v);
    }

    ll query() { return tree[1].len; }
};
