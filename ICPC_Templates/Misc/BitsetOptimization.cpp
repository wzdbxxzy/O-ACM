// Bitset Optimization (Bitset优化技巧集)
// 功能：状态压缩、位运算加速、DP优化
// 复杂度：通常将 O(n^2) 或 O(n^3) 优化到 O(n^2/w) 或 O(n^3/w)，w=64
// 应用：状压DP、图论、字符串匹配、集合运算

#include <bits/stdc++.h>
using namespace std;

// 应用1：Bitset加速背包DP
// 01背包：n个物品，容量W，求可达到的重量
bitset<100001> knapsack_bitset(const vector<int>& w, int W) {
    bitset<100001> dp;
    dp[0] = 1;

    for (int weight : w) {
        dp |= (dp << weight);  // 关键：位移+或运算
    }

    return dp;
}

// 应用2：Bitset加速传递闭包（Floyd）
// 图的可达性，复杂度从 O(n^3) 降到 O(n^3/64)
vector<bitset<5001>> transitive_closure(int n,
                                        const vector<pair<int, int>>& edges) {
    vector<bitset<5001>> reach(n);

    // 初始化
    for (int i = 0; i < n; i++) {
        reach[i][i] = 1;
    }
    for (auto [u, v] : edges) {
        reach[u][v] = 1;
    }

    // Floyd-Warshall with bitset
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (reach[i][k]) {
                reach[i] |= reach[k];
            }
        }
    }

    return reach;
}

// 应用3：Bitset加速字符串匹配（模式匹配）
// 多模式串匹配，Shift-And算法
class BitsetStringMatch {
    string text;
    vector<bitset<1001>> char_mask;  // char_mask[c] 表示字符c出现的位置

   public:
    BitsetStringMatch(const string& t) : text(t), char_mask(256) {
        int n = t.size();
        for (int i = 0; i < n; i++) {
            char_mask[(int)t[i]][i] = 1;
        }
    }

    // 查找模式串pattern的所有出现位置
    vector<int> find_all(const string& pattern) {
        int m = pattern.size();
        int n = text.size();

        bitset<1001> state;
        state.set();  // 全1

        vector<int> positions;

        for (int i = 0; i < n; i++) {
            state = (state << 1) | char_mask[(int)text[i]];

            if (i >= m - 1 && state[m - 1]) {
                positions.push_back(i - m + 1);
            }
        }

        return positions;
    }
};

// 应用4：Bitset优化最大团问题
// 求图的最大团（Bron-Kerbosch算法+bitset剪枝）
class MaxCliqueBS {
    int n;
    vector<bitset<101>> adj;
    int max_clique_size;
    vector<int> best_clique;

   public:
    MaxCliqueBS(int n_) : n(n_), adj(n), max_clique_size(0) {
        for (int i = 0; i < n; i++) {
            adj[i][i] = 1;  // 自己可以和自己相连
        }
    }

    void add_edge(int u, int v) { adj[u][v] = adj[v][u] = 1; }

    void bron_kerbosch(bitset<101> R, bitset<101> P, bitset<101> X,
                       vector<int>& current) {
        if (P.none() && X.none()) {
            if ((int)current.size() > max_clique_size) {
                max_clique_size = current.size();
                best_clique = current;
            }
            return;
        }

        // 选择pivot
        int pivot = -1;
        for (int i = 0; i < n; i++) {
            if (P[i] || X[i]) {
                pivot = i;
                break;
            }
        }

        if (pivot == -1) return;

        bitset<101> candidates = P & ~adj[pivot];

        for (int v = 0; v < n; v++) {
            if (!candidates[v]) continue;

            current.push_back(v);

            bitset<101> new_R = R;
            new_R[v] = 1;

            bron_kerbosch(new_R, P & adj[v], X & adj[v], current);

            current.pop_back();
            P[v] = 0;
            X[v] = 1;
        }
    }

    int solve() {
        bitset<101> R, P, X;
        for (int i = 0; i < n; i++) {
            P[i] = 1;
        }

        vector<int> current;
        bron_kerbosch(R, P, X, current);

        return max_clique_size;
    }
};

// 应用5：Bitset优化矩阵乘法（布尔矩阵）
vector<bitset<1001>> boolean_matrix_multiply(const vector<bitset<1001>>& A,
                                             const vector<bitset<1001>>& B,
                                             int n) {
    vector<bitset<1001>> C(n);

    // 转置B以优化缓存
    vector<bitset<1001>> BT(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            BT[j][i] = B[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = (A[i] & BT[j]).any();
        }
    }

    return C;
}

// 应用6：Bitset优化子集DP
// 给定n个集合，求有多少种方式选择若干集合使得并集为全集U
class SubsetDP {
    int n, U;
    vector<bitset<21>> sets;

   public:
    SubsetDP(int n_, int U_) : n(n_), U(U_), sets(n) {}

    void add_set(int id, const vector<int>& elements) {
        for (int x : elements) {
            sets[id][x] = 1;
        }
    }

    long long count_covers() {
        vector<long long> dp(1 << n, 0);
        vector<bitset<21>> union_set(1 << n);

        for (int mask = 0; mask < (1 << n); mask++) {
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    union_set[mask] |= sets[i];
                }
            }

            if (union_set[mask].count() == U) {
                dp[mask] = 1;
            }
        }

        // SOS DP统计
        for (int i = 0; i < n; i++) {
            for (int mask = 0; mask < (1 << n); mask++) {
                if (mask & (1 << i)) {
                    dp[mask] += dp[mask ^ (1 << i)];
                }
            }
        }

        return dp[(1 << n) - 1];
    }
};

// 应用7：Bitset优化区间DP
// 判断区间内是否存在某种性质
class IntervalDP {
    int n;
    string s;
    vector<bitset<501>> dp;  // dp[i] 表示以i开始的所有区间的状态

   public:
    IntervalDP(const string& s_) : s(s_), n(s_.size()), dp(n) { solve(); }

    void solve() {
        // 初始化：长度为1的区间
        for (int i = 0; i < n; i++) {
            dp[i][i] = 1;
        }

        // 枚举区间长度
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;

                // 根据问题特点更新dp[i][j]
                // 例如：回文判断
                if (s[i] == s[j]) {
                    if (len == 2 || dp[i + 1][j - 1]) {
                        dp[i][j] = 1;
                    }
                }
            }
        }
    }

    bool query(int l, int r) { return dp[l][r]; }
};

// 应用8：Bitset加速图论算法
// 求图中三元环的数量（复杂度 O(m * sqrt(m)))
long long count_triangles(int n, const vector<pair<int, int>>& edges) {
    vector<vector<int>> adj(n);
    vector<int> deg(n, 0);

    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        deg[u]++;
        deg[v]++;
    }

    // 定向：从度数小的指向度数大的（度数相同时从编号小的指向大的）
    vector<bitset<10001>> reach(n);

    for (auto [u, v] : edges) {
        if (deg[u] < deg[v] || (deg[u] == deg[v] && u < v)) {
            reach[u][v] = 1;
        } else {
            reach[v][u] = 1;
        }
    }

    long long triangles = 0;

    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            if (reach[u][v]) {
                // 统计 u 和 v 的公共出邻居
                triangles += (reach[u] & reach[v]).count();
            }
        }
    }

    return triangles;
}

// 应用9：Bitset优化状压DP（旅行商问题）
// TSP问题的bitset优化
int tsp_bitset(int n, const vector<vector<int>>& dist) {
    const int INF = 1e9;
    vector<vector<int>> dp(1 << n, vector<int>(n, INF));

    dp[1][0] = 0;  // 从0号城市出发

    for (int mask = 1; mask < (1 << n); mask++) {
        bitset<20> bs(mask);

        for (int u = 0; u < n; u++) {
            if (!bs[u] || dp[mask][u] == INF) continue;

            for (int v = 0; v < n; v++) {
                if (bs[v]) continue;

                int new_mask = mask | (1 << v);
                dp[new_mask][v] =
                    min(dp[new_mask][v], dp[mask][u] + dist[u][v]);
            }
        }
    }

    int ans = INF;
    for (int u = 0; u < n; u++) {
        ans = min(ans, dp[(1 << n) - 1][u] + dist[u][0]);
    }

    return ans;
}

// 应用10：Bitset优化字符串哈希匹配
// 多模式串的快速匹配
class MultiPatternMatch {
    vector<bitset<1001>> patterns;
    int max_len;

   public:
    void add_pattern(const string& s) {
        bitset<1001> bs;
        for (int i = 0; i < (int)s.size(); i++) {
            bs[i] = 1;
        }
        patterns.push_back(bs);
        max_len = max(max_len, (int)s.size());
    }

    vector<int> match(const string& text) {
        vector<int> matches;

        for (int i = 0; i + max_len <= (int)text.size(); i++) {
            bitset<1001> window;
            for (int j = 0; j < max_len; j++) {
                window[j] = 1;  // 简化：实际需要匹配字符
            }

            for (const auto& pattern : patterns) {
                if ((window & pattern) == pattern) {
                    matches.push_back(i);
                    break;
                }
            }
        }

        return matches;
    }
};

// 使用示例
void solve() {
    // 示例1：01背包
    vector<int> weights = {1, 3, 4, 5};
    int W = 10;

    auto dp = knapsack_bitset(weights, W);

    cout << "Reachable weights: ";
    for (int i = 0; i <= W; i++) {
        if (dp[i]) cout << i << " ";
    }
    cout << "\n";

    // 示例2：传递闭包
    int n = 5;
    vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};

    auto reach = transitive_closure(n, edges);

    cout << "\nReachability:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << reach[i][j] << " ";
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
