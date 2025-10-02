// Persistent Trie (可持久化字典树)
// 功能：维护历史版本的字典树，常用于01Trie求区间异或最值
// 复杂度：O(log V) per operation, V为值域
// 应用：区间异或最大值、第k大异或值

#include <bits/stdc++.h>
using namespace std;

const int MAXLOG = 30;  // 数值的最大位数

struct PersistentTrie {
    struct Node {
        int ch[2];  // 左右儿子（0和1）
        int cnt;    // 该节点代表的数出现次数

        Node() : cnt(0) { ch[0] = ch[1] = 0; }
    };

    vector<Node> nodes;
    vector<int> roots;  // 每个版本的根节点

    PersistentTrie() {
        nodes.push_back(Node());  // 0号节点为空节点
        roots.push_back(0);       // 初始版本（空树）
    }

    int new_node() {
        nodes.push_back(Node());
        return nodes.size() - 1;
    }

    // 在版本 ver 的基础上插入 x，返回新版本号
    int insert(int ver, int x, int delta = 1) {
        int new_root = _insert(roots[ver], x, MAXLOG - 1, delta);
        roots.push_back(new_root);
        return roots.size() - 1;
    }

    int _insert(int old, int x, int bit, int delta) {
        int cur = new_node();
        nodes[cur] = nodes[old];
        nodes[cur].cnt += delta;

        if (bit < 0) return cur;

        int d = (x >> bit) & 1;
        nodes[cur].ch[d] = _insert(nodes[old].ch[d], x, bit - 1, delta);

        return cur;
    }

    // 查询版本 [l, r] 之间插入的数中，与 x 异或的最大值
    int query_max_xor(int l, int r, int x) {
        return _query_max_xor(roots[l], roots[r], x, MAXLOG - 1);
    }

    int _query_max_xor(int left_ver, int right_ver, int x, int bit) {
        if (bit < 0) return 0;

        int d = (x >> bit) & 1;
        int want = d ^ 1;  // 希望走的方向

        // 检查区间内是否有数走want方向
        int cnt_want = nodes[nodes[right_ver].ch[want]].cnt -
                       nodes[nodes[left_ver].ch[want]].cnt;

        if (cnt_want > 0) {
            return (1 << bit) + _query_max_xor(nodes[left_ver].ch[want],
                                               nodes[right_ver].ch[want], x,
                                               bit - 1);
        } else {
            return _query_max_xor(nodes[left_ver].ch[d], nodes[right_ver].ch[d],
                                  x, bit - 1);
        }
    }

    // 查询版本 [l, r] 之间插入的数中，与 x 异或的最小值
    int query_min_xor(int l, int r, int x) {
        return _query_min_xor(roots[l], roots[r], x, MAXLOG - 1);
    }

    int _query_min_xor(int left_ver, int right_ver, int x, int bit) {
        if (bit < 0) return 0;

        int d = (x >> bit) & 1;

        // 优先走相同方向
        int cnt_same = nodes[nodes[right_ver].ch[d]].cnt -
                       nodes[nodes[left_ver].ch[d]].cnt;

        if (cnt_same > 0) {
            return _query_min_xor(nodes[left_ver].ch[d], nodes[right_ver].ch[d],
                                  x, bit - 1);
        } else {
            return (1 << bit) + _query_min_xor(nodes[left_ver].ch[d ^ 1],
                                               nodes[right_ver].ch[d ^ 1], x,
                                               bit - 1);
        }
    }

    // 查询版本 ver 中小于等于 x 的数的个数
    int count_le(int ver, int x) {
        return _count_le(roots[ver], x, MAXLOG - 1);
    }

    int _count_le(int u, int x, int bit) {
        if (bit < 0 || u == 0) return nodes[u].cnt;

        int d = (x >> bit) & 1;
        int res = 0;

        if (d == 1) {
            // 如果当前位是1，左子树（0）的所有数都小于x
            res += nodes[nodes[u].ch[0]].cnt;
        }

        res += _count_le(nodes[u].ch[d], x, bit - 1);
        return res;
    }

    // 查询版本 [l, r] 之间插入的数中的第 k 小（k从1开始）
    int kth_smallest(int l, int r, int k) {
        return _kth_smallest(roots[l], roots[r], k, MAXLOG - 1);
    }

    int _kth_smallest(int left_ver, int right_ver, int k, int bit) {
        if (bit < 0) return 0;

        int left_cnt = nodes[nodes[right_ver].ch[0]].cnt -
                       nodes[nodes[left_ver].ch[0]].cnt;

        if (k <= left_cnt) {
            return _kth_smallest(nodes[left_ver].ch[0], nodes[right_ver].ch[0],
                                 k, bit - 1);
        } else {
            return (1 << bit) + _kth_smallest(nodes[left_ver].ch[1],
                                              nodes[right_ver].ch[1],
                                              k - left_cnt, bit - 1);
        }
    }
};

// 使用示例1：区间异或最大值
void example1() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    // 构建前缀异或和的可持久化Trie
    PersistentTrie trie;
    int prefix_xor = 0;

    for (int i = 1; i <= n; i++) {
        prefix_xor ^= a[i];
        trie.insert(i - 1, prefix_xor);
    }

    // 查询
    for (int i = 0; i < m; i++) {
        int l, r, x;
        cin >> l >> r >> x;

        // 区间 [l, r] 的子区间异或最大值
        // 等价于查询前缀异或和 [l-1, r] 中与 pre[l-1]^x 异或的最大值
        int ans = trie.query_max_xor(l - 1, r, x);
        cout << ans << "\n";
    }
}

// 使用示例2：静态区间第k小
void example2() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    PersistentTrie trie;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        trie.insert(i - 1, a[i]);
    }

    for (int i = 0; i < m; i++) {
        int l, r, k;
        cin >> l >> r >> k;

        int ans = trie.kth_smallest(l - 1, r, k);
        cout << ans << "\n";
    }
}

// 使用示例3：区间内与x异或后第k小
void example3() {
    int n;
    cin >> n;

    vector<int> a(n + 1);
    PersistentTrie trie;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        trie.insert(i - 1, a[i]);
    }

    int q;
    cin >> q;
    while (q--) {
        int l, r, x, k;
        cin >> l >> r >> x >> k;

        // 区间[l,r]内所有数与x异或后的第k小
        // 需要二分答案 + 可持久化Trie计数
        int left = 0, right = (1 << MAXLOG) - 1;
        int ans = 0;

        // 通过二分 + count_le 找第k小
        while (left <= right) {
            int mid = (left + right) / 2;
            int cnt = trie.count_le(r, mid ^ x) - trie.count_le(l - 1, mid ^ x);

            if (cnt >= k) {
                ans = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        cout << (ans ^ x) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    example1();
    // example2();
    // example3();

    return 0;
}
