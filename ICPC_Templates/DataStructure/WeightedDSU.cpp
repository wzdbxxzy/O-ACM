// Weighted Disjoint Set Union (带权并查集)
// 功能：维护集合内元素的相对关系
// 复杂度：O(α(n)) 均摊
// 应用：关系判断、种类并查集、偏移量维护

#include <bits/stdc++.h>
using namespace std;

// 版本1：加法关系（偏移量）
struct WeightedDSU {
    vector<int> f;
    vector<long long> w;  // w[x] 表示 x 到 f[x] 的权值

    WeightedDSU(int n) {
        f.resize(n);
        iota(f.begin(), f.end(), 0);
        w.assign(n, 0);
    }

    // 返回 {根节点, x到根的权值}
    pair<int, long long> find(int x) {
        if (x == f[x]) {
            return {x, 0};
        }
        auto [root, weight] = find(f[x]);
        f[x] = root;
        w[x] += weight;
        return {root, w[x]};
    }

    // 合并：使得 x 的权值 = y 的权值 + val
    // 即 w[x] - w[y] = val
    bool merge(int x, int y, long long val) {
        auto [rx, wx] = find(x);
        auto [ry, wy] = find(y);

        if (rx == ry) {
            return (wx - wy) == val;
        }

        // 合并：rx -> ry
        // 需要满足：wx + w[rx] = wy + val
        f[rx] = ry;
        w[rx] = wy + val - wx;
        return true;
    }

    // 查询 x 和 y 的权值差
    // 返回 w[x] - w[y]，如果不在同一集合返回 nullopt
    optional<long long> diff(int x, int y) {
        auto [rx, wx] = find(x);
        auto [ry, wy] = find(y);

        if (rx != ry) {
            return nullopt;
        }
        return wx - wy;
    }
};

// 版本2：种类并查集（扩展域法）
// 应用：食物链问题（A吃B，B吃C，C吃A）
struct TypeDSU {
    vector<int> f;
    int n;  // 原始节点数

    // 使用3倍空间：[0, n) 同类，[n, 2n) 吃我的，[2n, 3n) 我吃的
    TypeDSU(int n_) : n(n_) {
        f.resize(n * 3);
        iota(f.begin(), f.end(), 0);
    }

    int find(int x) { return f[x] == x ? x : f[x] = find(f[x]); }

    void merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x != y) f[x] = y;
    }

    // x 和 y 是同类
    void same_type(int x, int y) {
        merge(x, y);
        merge(x + n, y + n);
        merge(x + 2 * n, y + 2 * n);
    }

    // x 吃 y
    void eat(int x, int y) {
        merge(x, y + 2 * n);      // x 和 "y的猎物" 同类
        merge(x + n, y);          // "x的天敌" 和 y 同类
        merge(x + 2 * n, y + n);  // "x的猎物" 和 "y的天敌" 同类
    }

    // 判断 x 和 y 是否同类
    bool is_same(int x, int y) { return find(x) == find(y); }

    // 判断 x 是否吃 y
    bool is_eat(int x, int y) { return find(x) == find(y + 2 * n); }

    // 判断 x 和 y 是否矛盾
    bool is_conflict(int x, int y, int type) {
        if (type == 1) {  // 声称同类
            return find(x + n) == find(y) || find(x) == find(y + n);
        } else {  // 声称 x 吃 y
            return find(x) == find(y) || find(x) == find(y + n);
        }
    }
};

// 版本3：区间并查集（维护区间染色）
struct IntervalDSU {
    map<int, int> f;

    int find(int x) {
        if (!f.count(x)) return f[x] = x;
        return f[x] == x ? x : f[x] = find(f[x]);
    }

    // 将 [l, r] 区间合并（染色）
    vector<int> merge_interval(int l, int r) {
        vector<int> merged;
        int cur = find(l);

        while (cur <= r) {
            merged.push_back(cur);
            int next = find(cur + 1);
            f[cur] = next;
            cur = next;
        }

        return merged;
    }

    // 查询 x 所在的未被覆盖的最左位置
    int query(int x) { return find(x); }
};

// 使用示例1：偏移量问题
void example1() {
    int n = 5;
    WeightedDSU dsu(n);

    // a[1] - a[0] = 3
    dsu.merge(1, 0, 3);

    // a[2] - a[1] = 2
    dsu.merge(2, 1, 2);

    // 查询 a[2] - a[0]
    auto diff = dsu.diff(2, 0);
    if (diff) {
        cout << "a[2] - a[0] = " << *diff << "\n";  // 输出 5
    }
}

// 使用示例2：食物链（种类并查集）
void example2() {
    int n, m;
    cin >> n >> m;

    TypeDSU dsu(n);
    int ans = 0;

    for (int i = 0; i < m; i++) {
        int type, x, y;
        cin >> type >> x >> y;
        x--;
        y--;

        if (x >= n || y >= n) {
            ans++;
            continue;
        }

        if (type == 1) {  // x 和 y 同类
            if (dsu.is_eat(x, y) || dsu.is_eat(y, x)) {
                ans++;
            } else {
                dsu.same_type(x, y);
            }
        } else {  // x 吃 y
            if (dsu.is_same(x, y) || dsu.is_eat(y, x)) {
                ans++;
            } else {
                dsu.eat(x, y);
            }
        }
    }

    cout << ans << "\n";
}

// 使用示例3：区间染色
void example3() {
    int n, m;
    cin >> n >> m;

    IntervalDSU dsu;

    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;

        auto merged = dsu.merge_interval(l, r);
        cout << "Merged intervals: ";
        for (int x : merged) {
            cout << x << " ";
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // example1();
    example2();
    // example3();

    return 0;
}
