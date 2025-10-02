// Li Chao Segment Tree (李超线段树)
// 功能：动态维护一组直线，查询某点的最值
// 复杂度：插入 O(log n), 查询 O(log n)
// 应用：斜率优化DP、动态凸包

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;
const long long INF = 1e18;

struct Line {
    long long k, b;  // y = k * x + b

    Line(long long k_ = 0, long long b_ = INF) : k(k_), b(b_) {}

    long long calc(long long x) const { return k * x + b; }
};

struct LiChaoTree {
    Line tree[MAXN * 4];
    int n;

    void init(int n_) {
        n = n_;
        for (int i = 0; i < n * 4; i++) {
            tree[i] = Line();
        }
    }

    // 插入线段 [L, R] 的直线
    void insert(int p, int l, int r, int L, int R, Line line) {
        if (L <= l && r <= R) {
            insert_line(p, l, r, line);
            return;
        }

        int mid = (l + r) >> 1;
        if (L <= mid) insert(p * 2, l, mid, L, R, line);
        if (mid < R) insert(p * 2 + 1, mid + 1, r, L, R, line);
    }

    // 在整个区间插入直线
    void insert_line(int p, int l, int r, Line line) {
        int mid = (l + r) >> 1;

        bool lef = line.calc(l) < tree[p].calc(l);
        bool mid_less = line.calc(mid) < tree[p].calc(mid);

        if (mid_less) swap(tree[p], line);
        if (l == r) return;

        if (lef != mid_less) {
            insert_line(p * 2, l, mid, line);
        } else {
            insert_line(p * 2 + 1, mid + 1, r, line);
        }
    }

    // 查询点 x 的最小值
    long long query(int p, int l, int r, int x) {
        if (l == r) return tree[p].calc(x);

        int mid = (l + r) >> 1;
        long long res = tree[p].calc(x);

        if (x <= mid) {
            res = min(res, query(p * 2, l, mid, x));
        } else {
            res = min(res, query(p * 2 + 1, mid + 1, r, x));
        }

        return res;
    }

    // 外部接口
    void add_line(long long k, long long b) {
        insert(1, 0, n - 1, 0, n - 1, Line(k, b));
    }

    void add_segment(int l, int r, long long k, long long b) {
        insert(1, 0, n - 1, l, r, Line(k, b));
    }

    long long query(int x) { return query(1, 0, n - 1, x); }
};

// 可持久化李超线段树（Persistent Li Chao Tree）
struct PersistentLiChaoTree {
    struct Node {
        int lson, rson;
        Line line;

        Node() : lson(0), rson(0), line() {}
    };

    vector<Node> nodes;
    vector<int> roots;
    int n;

    PersistentLiChaoTree(int n_) : n(n_) {
        nodes.reserve(MAXN * 20);
        nodes.push_back(Node());  // 空节点
        roots.push_back(0);       // 初始版本
    }

    int new_node() {
        nodes.push_back(Node());
        return nodes.size() - 1;
    }

    int insert_line(int old, int l, int r, Line line) {
        int cur = new_node();
        nodes[cur] = nodes[old];

        int mid = (l + r) >> 1;

        bool lef = line.calc(l) < nodes[cur].line.calc(l);
        bool mid_less = line.calc(mid) < nodes[cur].line.calc(mid);

        if (mid_less) swap(nodes[cur].line, line);

        if (l < r) {
            if (lef != mid_less) {
                nodes[cur].lson = insert_line(nodes[old].lson, l, mid, line);
            } else {
                nodes[cur].rson =
                    insert_line(nodes[old].rson, mid + 1, r, line);
            }
        }

        return cur;
    }

    long long query(int p, int l, int r, int x) {
        if (p == 0) return INF;

        long long res = nodes[p].line.calc(x);
        if (l == r) return res;

        int mid = (l + r) >> 1;
        if (x <= mid) {
            res = min(res, query(nodes[p].lson, l, mid, x));
        } else {
            res = min(res, query(nodes[p].rson, mid + 1, r, x));
        }

        return res;
    }

    // 在版本 ver 的基础上添加直线，返回新版本号
    int add_line(int ver, long long k, long long b) {
        int new_root = insert_line(roots[ver], 0, n - 1, Line(k, b));
        roots.push_back(new_root);
        return roots.size() - 1;
    }

    long long query(int ver, int x) { return query(roots[ver], 0, n - 1, x); }
};

// 使用示例
void solve() {
    int n, q;
    cin >> n >> q;

    LiChaoTree lct;
    lct.init(100000);

    while (q--) {
        int op;
        cin >> op;

        if (op == 1) {
            // 添加直线 y = kx + b
            long long k, b;
            cin >> k >> b;
            lct.add_line(k, b);
        } else {
            // 查询 x 处的最小值
            int x;
            cin >> x;
            cout << lct.query(x) << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
