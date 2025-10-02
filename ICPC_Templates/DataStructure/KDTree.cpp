// KD Tree (K维树)
// 功能：多维空间中的最近邻查询、范围查询
// 复杂度：构建 O(n log n), 查询平均 O(log n), 最坏 O(n)
// 应用：最近邻搜索、k近邻、范围统计

#include <bits/stdc++.h>
using namespace std;

const int K = 2;  // 维度（可修改）
const int MAXN = 2e5 + 10;
const long long INF = 1e18;

struct Point {
    int x[K];  // K维坐标
    int id;    // 点的编号

    long long dist(const Point& p) const {
        long long res = 0;
        for (int i = 0; i < K; i++) {
            res += 1LL * (x[i] - p.x[i]) * (x[i] - p.x[i]);
        }
        return res;
    }
};

struct KDTree {
    Point pts[MAXN];
    int split[MAXN];  // 每个节点的分割维度
    int n;

    // 按第 d 维排序
    static int cmp_dim;
    static bool cmp(const Point& a, const Point& b) {
        return a.x[cmp_dim] < b.x[cmp_dim];
    }

    // 构建KD树
    void build(int l, int r) {
        if (l >= r) return;

        int mid = (l + r) >> 1;

        // 选择方差最大的维度作为分割维度
        double max_var = -1;
        split[mid] = -1;

        for (int d = 0; d < K; d++) {
            double avg = 0;
            for (int i = l; i <= r; i++) {
                avg += pts[i].x[d];
            }
            avg /= (r - l + 1);

            double var = 0;
            for (int i = l; i <= r; i++) {
                var += (pts[i].x[d] - avg) * (pts[i].x[d] - avg);
            }
            var /= (r - l + 1);

            if (var > max_var) {
                max_var = var;
                split[mid] = d;
            }
        }

        cmp_dim = split[mid];
        nth_element(pts + l, pts + mid, pts + r + 1, cmp);

        build(l, mid - 1);
        build(mid + 1, r);
    }

    // 查询最近点
    int ans_id;
    long long ans_dist;

    void query(int l, int r, const Point& p) {
        if (l > r) return;

        int mid = (l + r) >> 1;
        long long d = pts[mid].dist(p);

        // 更新答案
        if (d < ans_dist || (d == ans_dist && pts[mid].id < ans_id)) {
            ans_dist = d;
            ans_id = pts[mid].id;
        }

        // 剪枝：先搜索更可能包含答案的子树
        int dim = split[mid];
        long long delta =
            1LL * (p.x[dim] - pts[mid].x[dim]) * (p.x[dim] - pts[mid].x[dim]);

        if (p.x[dim] < pts[mid].x[dim]) {
            query(l, mid - 1, p);
            if (ans_dist > delta) {
                query(mid + 1, r, p);
            }
        } else {
            query(mid + 1, r, p);
            if (ans_dist > delta) {
                query(l, mid - 1, p);
            }
        }
    }

    Point find_nearest(const Point& p) {
        ans_dist = INF;
        ans_id = -1;
        query(0, n - 1, p);
        return pts[ans_id];
    }
};

int KDTree::cmp_dim = 0;

// 使用示例
void solve() {
    KDTree tree;
    cin >> tree.n;

    for (int i = 0; i < tree.n; i++) {
        for (int j = 0; j < K; j++) {
            cin >> tree.pts[i].x[j];
        }
        tree.pts[i].id = i;
    }

    tree.build(0, tree.n - 1);

    int q;
    cin >> q;
    while (q--) {
        Point p;
        for (int i = 0; i < K; i++) {
            cin >> p.x[i];
        }

        Point nearest = tree.find_nearest(p);
        for (int i = 0; i < K; i++) {
            cout << nearest.x[i] << " ";
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
