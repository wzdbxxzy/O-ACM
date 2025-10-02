// Prufer Sequence (Prufer序列)
// 功能：树与序列的双射、树的计数
// 复杂度：O(n) 或 O(n log n)
// 应用：带标号树计数、生成随机树、Cayley定理

#include <bits/stdc++.h>
using namespace std;

// Prufer序列：n个带标号节点的树 ↔ 长度为n-2的序列（值域[0, n-1]）

// 树 → Prufer序列（O(n log n)版本，使用优先队列）
vector<int> tree_to_prufer(int n, const vector<pair<int, int>>& edges) {
    vector<vector<int>> adj(n);
    vector<int> degree(n, 0);

    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    priority_queue<int, vector<int>, greater<int>> leaves;

    for (int i = 0; i < n; i++) {
        if (degree[i] == 1) {
            leaves.push(i);
        }
    }

    vector<int> prufer;

    for (int i = 0; i < n - 2; i++) {
        int leaf = leaves.top();
        leaves.pop();

        int neighbor = -1;
        for (int v : adj[leaf]) {
            if (degree[v] > 0) {
                neighbor = v;
                break;
            }
        }

        prufer.push_back(neighbor);
        degree[leaf]--;
        degree[neighbor]--;

        if (degree[neighbor] == 1) {
            leaves.push(neighbor);
        }
    }

    return prufer;
}

// Prufer序列 → 树（O(n)版本）
vector<pair<int, int>> prufer_to_tree(const vector<int>& prufer) {
    int n = prufer.size() + 2;
    vector<int> degree(n, 1);

    for (int x : prufer) {
        degree[x]++;
    }

    vector<pair<int, int>> edges;
    int ptr = 0;

    for (int i = 0; i < n - 2; i++) {
        // 找到最小的度为1的节点
        while (degree[ptr] != 1) {
            ptr++;
        }

        int leaf = ptr;
        int v = prufer[i];

        edges.push_back({leaf, v});

        degree[leaf]--;
        degree[v]--;
    }

    // 最后两个度为1的节点连边
    vector<int> remaining;
    for (int i = 0; i < n; i++) {
        if (degree[i] == 1) {
            remaining.push_back(i);
        }
    }

    edges.push_back({remaining[0], remaining[1]});

    return edges;
}

// Cayley定理：n个带标号节点的树的数量 = n^(n-2)
ll count_labeled_trees(int n) {
    ll result = 1;
    for (int i = 0; i < n - 2; i++) {
        result *= n;
    }
    return result;
}

// 计算度数序列为d的带标号树的数量
// 公式：(n-2)! / ∏(d[i] - 1)!
ll count_trees_with_degree(const vector<int>& degree) {
    int n = degree.size();

    // 检查度数和是否为2(n-1)
    int sum = 0;
    for (int d : degree) {
        sum += d;
    }
    if (sum != 2 * (n - 1)) return 0;

    // 计算阶乘
    vector<ll> fact(n);
    fact[0] = 1;
    for (int i = 1; i < n; i++) {
        fact[i] = fact[i - 1] * i;
    }

    ll numerator = fact[n - 2];
    ll denominator = 1;

    for (int d : degree) {
        if (d < 1) return 0;
        denominator *= fact[d - 1];
    }

    return numerator / denominator;
}

// 生成随机树
vector<pair<int, int>> random_tree(int n) {
    vector<int> prufer(n - 2);

    for (int i = 0; i < n - 2; i++) {
        prufer[i] = rand() % n;
    }

    return prufer_to_tree(prufer);
}

// 应用1：生成所有n个节点的树
void generate_all_trees(int n) {
    // 枚举所有长度为n-2的序列
    function<void(vector<int>&)> enumerate = [&](vector<int>& prufer) {
        if ((int)prufer.size() == n - 2) {
            auto tree = prufer_to_tree(prufer);

            // 输出树
            cout << "Prufer: ";
            for (int x : prufer) {
                cout << x << " ";
            }
            cout << "\nEdges: ";
            for (auto [u, v] : tree) {
                cout << "(" << u << "," << v << ") ";
            }
            cout << "\n\n";

            return;
        }

        for (int i = 0; i < n; i++) {
            prufer.push_back(i);
            enumerate(prufer);
            prufer.pop_back();
        }
    };

    vector<int> prufer;
    enumerate(prufer);
}

// 应用2：判断是否为树的同构
// 通过比较规范化的Prufer序列
bool is_isomorphic(const vector<pair<int, int>>& tree1,
                   const vector<pair<int, int>>& tree2) {
    if (tree1.size() != tree2.size()) return false;

    int n = tree1.size() + 1;

    auto prufer1 = tree_to_prufer(n, tree1);
    auto prufer2 = tree_to_prufer(n, tree2);

    // 规范化：对Prufer序列排序
    sort(prufer1.begin(), prufer1.end());
    sort(prufer2.begin(), prufer2.end());

    return prufer1 == prufer2;
}

// 应用3：计算特定结构的树的数量
// 例如：星形树（一个中心节点连接所有其他节点）
ll count_star_trees(int n, int center) {
    // 星形树的Prufer序列全是center
    return 1;  // 只有一种
}

// 应用4：带约束的树计数
// 例如：边(u, v)必须存在
ll count_trees_with_edge(int n, int u, int v) {
    // 在Prufer序列中，u或v至少出现一次（或都不出现但它们是最后两个叶子）

    // 总数
    ll total = count_labeled_trees(n);

    // u和v都不在Prufer序列中的数量（u和v是最后的叶子）
    // 这种情况下，除了u、v外的n-2个节点构成Prufer序列
    ll without = 1;
    for (int i = 0; i < n - 2; i++) {
        without *= (n - 2);
    }

    return total - without + 1;  // +1是因为u-v恰好是最后一条边的情况
}

// 应用5：树的重心相关
// 利用Prufer序列性质：节点i在Prufer序列中出现d[i]-1次
vector<int> tree_degrees_from_prufer(const vector<int>& prufer) {
    int n = prufer.size() + 2;
    vector<int> degree(n, 1);

    for (int x : prufer) {
        degree[x]++;
    }

    return degree;
}

void solve() {
    // 示例1：从树生成Prufer序列
    int n = 6;
    vector<pair<int, int>> tree = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}};

    auto prufer = tree_to_prufer(n, tree);

    cout << "Original tree edges:\n";
    for (auto [u, v] : tree) {
        cout << u << " - " << v << "\n";
    }

    cout << "\nPrufer sequence: ";
    for (int x : prufer) {
        cout << x << " ";
    }
    cout << "\n\n";

    // 示例2：从Prufer序列恢复树
    auto reconstructed = prufer_to_tree(prufer);

    cout << "Reconstructed tree edges:\n";
    for (auto [u, v] : reconstructed) {
        cout << u << " - " << v << "\n";
    }

    // 示例3：计算带标号树的数量
    cout << "\nNumber of labeled trees with 6 nodes: " << count_labeled_trees(6)
         << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
