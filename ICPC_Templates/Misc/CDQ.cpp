/**
 * CDQ分治
 * 功能：处理三维偏序问题
 * 复杂度：O(n log^2 n)
 */
#include <bits/stdc++.h>
using namespace std;

// 三维偏序问题
struct Point {
    int x, y, z;
    int cnt;  // 相同点的个数
    int ans;  // 答案
    int id;

    bool operator<(const Point& p) const {
        if (x != p.x) return x < p.x;
        if (y != p.y) return y < p.y;
        return z < p.z;
    }
};

struct CDQ {
    vector<Point> a;
    vector<int> c;  // 树状数组
    int maxz;

    CDQ(vector<Point> points, int maxz) : a(points), maxz(maxz) {
        c.resize(maxz + 1);
    }

    void add(int x, int v) {
        for (int i = x; i <= maxz; i += i & -i) {
            c[i] += v;
        }
    }

    int query(int x) {
        int res = 0;
        for (int i = x; i > 0; i -= i & -i) {
            res += c[i];
        }
        return res;
    }

    void cdq(int l, int r) {
        if (l >= r) return;

        int mid = (l + r) / 2;
        cdq(l, mid);
        cdq(mid + 1, r);

        // 归并排序，按y排序
        sort(a.begin() + l, a.begin() + mid + 1,
             [](const Point& p1, const Point& p2) { return p1.y < p2.y; });
        sort(a.begin() + mid + 1, a.begin() + r + 1,
             [](const Point& p1, const Point& p2) { return p1.y < p2.y; });

        int i = l, j = mid + 1;
        while (j <= r) {
            while (i <= mid && a[i].y <= a[j].y) {
                add(a[i].z, a[i].cnt);
                i++;
            }
            a[j].ans += query(a[j].z);
            j++;
        }

        // 清空树状数组
        for (int k = l; k < i; k++) {
            add(a[k].z, -a[k].cnt);
        }
    }

    vector<int> solve() {
        // 去重并统计
        sort(a.begin(), a.end());
        vector<Point> b;
        for (int i = 0; i < a.size();) {
            int j = i;
            while (j < a.size() && a[j].x == a[i].x && a[j].y == a[i].y &&
                   a[j].z == a[i].z) {
                j++;
            }
            Point p = a[i];
            p.cnt = j - i;
            b.push_back(p);
            i = j;
        }

        a = b;
        cdq(0, a.size() - 1);

        // 收集答案
        vector<int> ans(a.size());
        for (int i = 0; i < a.size(); i++) {
            ans[a[i].id] = a[i].ans;
        }
        return ans;
    }
};
