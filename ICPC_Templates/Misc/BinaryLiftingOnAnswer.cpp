// Binary Lifting on Answer / Parallel Binary Search (整体二分)
// 功能：同时对多个查询进行二分答案
// 复杂度：O(n log n log V)
// 应用：离线区间第k小、动态加点后的查询

#include <bits/stdc++.h>
using namespace std;

// 整体二分框架
template <typename Query, typename Update>
struct ParallelBinarySearch {
    int n, m;  // n个修改操作，m个查询
    vector<Update> updates;
    vector<Query> queries;
    vector<int> answer;

    ParallelBinarySearch(int n, int m) : n(n), m(m) { answer.resize(m, -1); }

    // 添加修改操作
    void add_update(const Update& u) { updates.push_back(u); }

    // 添加查询
    void add_query(const Query& q) { queries.push_back(q); }

    // 核心：判断query在当前修改范围[0, mid]下是否满足条件
    // 需要根据具体问题实现
    virtual bool check(const Query& q, int mid) = 0;

    // 应用修改操作
    virtual void apply(const Update& u) = 0;

    // 撤销修改操作
    virtual void undo(const Update& u) = 0;

    // 整体二分主过程
    void solve(int l, int r, vector<int> query_ids) {
        if (l > r || query_ids.empty()) return;

        if (l == r) {
            for (int id : query_ids) {
                answer[id] = l;
            }
            return;
        }

        int mid = (l + r) / 2;

        // 应用 [l, mid] 的所有修改
        for (int i = l; i <= mid; i++) {
            if (i < (int)updates.size()) {
                apply(updates[i]);
            }
        }

        // 分组查询
        vector<int> left_queries, right_queries;

        for (int id : query_ids) {
            if (check(queries[id], mid)) {
                left_queries.push_back(id);
            } else {
                right_queries.push_back(id);
            }
        }

        // 撤销修改
        for (int i = mid; i >= l; i--) {
            if (i < (int)updates.size()) {
                undo(updates[i]);
            }
        }

        // 递归处理
        solve(l, mid, left_queries);
        solve(mid + 1, r, right_queries);
    }

    void run() {
        vector<int> all_queries(m);
        iota(all_queries.begin(), all_queries.end(), 0);
        solve(0, n, all_queries);
    }
};

// ========== 应用示例 ==========

// 示例1：动态区间第k小
struct RangeKthQuery {
    int l, r, k;
};

struct PointUpdate {
    int pos;
    int val;
};

struct RangeKthSolver : ParallelBinarySearch<RangeKthQuery, PointUpdate> {
    vector<int> bit;
    int maxn;

    RangeKthSolver(int n, int m, int maxn_)
        : ParallelBinarySearch(n, m), maxn(maxn_) {
        bit.resize(maxn + 1, 0);
    }

    void update(int pos, int delta) {
        for (int i = pos; i <= maxn; i += i & -i) {
            bit[i] += delta;
        }
    }

    int query(int pos) {
        int res = 0;
        for (int i = pos; i > 0; i -= i & -i) {
            res += bit[i];
        }
        return res;
    }

    bool check(const RangeKthQuery& q, int mid) override {
        // 查询区间[q.l, q.r]中值<=mid的数的个数
        int cnt = query(q.r) - query(q.l - 1);
        return cnt >= q.k;
    }

    void apply(const PointUpdate& u) override { update(u.pos, 1); }

    void undo(const PointUpdate& u) override { update(u.pos, -1); }
};

// 示例2：带修改的区间众数
struct ModifiableMode {
    struct Query {
        int l, r, time;  // time表示在第几次修改后查询
    };

    struct Modification {
        int pos, val;
    };

    int n, q;
    vector<int> a;
    vector<Modification> mods;
    vector<Query> queries;
    map<int, int> cnt;
    int mode_val, mode_cnt;

    void add(int val) {
        cnt[val]++;
        if (cnt[val] > mode_cnt || (cnt[val] == mode_cnt && val < mode_val)) {
            mode_val = val;
            mode_cnt = cnt[val];
        }
    }

    void remove(int val) {
        cnt[val]--;
        if (cnt[val] == 0) cnt.erase(val);
        // 重新计算众数
        mode_cnt = 0;
        mode_val = 1e9;
        for (auto [v, c] : cnt) {
            if (c > mode_cnt || (c == mode_cnt && v < mode_val)) {
                mode_val = v;
                mode_cnt = c;
            }
        }
    }

    void solve_queries() {
        // 使用整体二分或莫队算法
        // 这里给出莫队的框架
        // ... 具体实现
    }
};

// 示例3：区间加、单点查询历史版本
struct PersistentRangeAdd {
    struct Update {
        int l, r, val;
    };

    struct Query {
        int pos, time;  // 查询位置pos在time时刻的值
    };

    vector<Update> updates;
    vector<Query> queries;
    vector<int> answers;

    // 使用整体二分来处理
    void solve() {
        // 按时间排序所有操作
        // 使用差分数组 + 整体二分
        // ... 具体实现
    }
};

// 使用示例：动态区间第k小
void example1() {
    int n, m;
    cin >> n >> m;

    RangeKthSolver solver(n, m, n);

    // 读入修改（插入数字）
    for (int i = 0; i < n; i++) {
        int pos, val;
        cin >> pos >> val;
        solver.add_update({pos, val});
    }

    // 读入查询
    for (int i = 0; i < m; i++) {
        int l, r, k;
        cin >> l >> r >> k;
        solver.add_query({l, r, k});
    }

    solver.run();

    // 输出答案
    for (int i = 0; i < m; i++) {
        cout << solver.answer[i] << "\n";
    }
}

// 经典题目：[ZJOI2013]K大数查询
// n个位置，m个操作
// 操作1：区间[l,r]加入c个数字x
// 操作2：查询区间[l,r]第k大数
void zjoi2013() {
    int n, m;
    cin >> n >> m;

    struct Op {
        int type, l, r, c;
    };

    vector<Op> ops(m);
    for (int i = 0; i < m; i++) {
        cin >> ops[i].type >> ops[i].l >> ops[i].r >> ops[i].c;
    }

    // 整体二分 + 树状数组
    // 二分答案的值域
    // 对于每个查询，二分它的答案

    // ... 具体实现需要结合树状数组维护区间加
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    example1();

    return 0;
}
