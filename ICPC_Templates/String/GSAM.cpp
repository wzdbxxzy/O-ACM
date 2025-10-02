// Generalized Suffix Automaton (广义后缀自动机)
// 功能：多串后缀自动机，统计本质不同子串
// 复杂度：O(Σ|s_i|)
// 应用：多串匹配、公共子串

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e6 + 10;
const int CHARSET = 26;

struct GSAM {
    int son[MAXN][CHARSET];
    int len[MAXN];
    int link[MAXN];
    int cnt;

    void init() {
        cnt = 0;
        link[0] = -1;
        memset(son[0], 0, sizeof(son[0]));
    }

    // 插入一个字符，返回新状态
    int insert(int c, int last) {
        if (son[last][c]) {
            // 状态已存在
            int q = son[last][c];
            if (len[q] == len[last] + 1) {
                return q;
            } else {
                // 需要分裂
                int clone = ++cnt;
                len[clone] = len[last] + 1;
                link[clone] = link[q];
                memcpy(son[clone], son[q], sizeof(son[q]));

                while (last != -1 && son[last][c] == q) {
                    son[last][c] = clone;
                    last = link[last];
                }

                link[q] = clone;
                return clone;
            }
        } else {
            // 新建状态
            int p = ++cnt;
            len[p] = len[last] + 1;

            while (last != -1 && !son[last][c]) {
                son[last][c] = p;
                last = link[last];
            }

            if (last == -1) {
                link[p] = 0;
            } else {
                int q = son[last][c];
                if (len[q] == len[last] + 1) {
                    link[p] = q;
                } else {
                    int clone = ++cnt;
                    len[clone] = len[last] + 1;
                    link[clone] = link[q];
                    memcpy(son[clone], son[q], sizeof(son[q]));

                    while (last != -1 && son[last][c] == q) {
                        son[last][c] = clone;
                        last = link[last];
                    }

                    link[q] = link[p] = clone;
                }
            }

            return p;
        }
    }

    // 统计本质不同子串数量
    long long count_substrings() {
        long long ans = 0;
        for (int i = 1; i <= cnt; i++) {
            ans += len[i] - len[link[i]];
        }
        return ans;
    }

    // 为每个状态计算endpos集合大小
    vector<int> calculate_endpos() {
        vector<int> cnt_endpos(cnt + 1, 0);
        vector<int> bucket(cnt + 2, 0);
        vector<int> order(cnt + 1);

        // 桶排序
        for (int i = 1; i <= cnt; i++) {
            bucket[len[i]]++;
        }
        for (int i = 1; i <= cnt + 1; i++) {
            bucket[i] += bucket[i - 1];
        }
        for (int i = 1; i <= cnt; i++) {
            order[--bucket[len[i]]] = i;
        }

        // 从长度大到小合并
        for (int i = cnt - 1; i >= 0; i--) {
            int p = order[i];
            if (link[p] != -1) {
                cnt_endpos[link[p]] += cnt_endpos[p];
            }
        }

        return cnt_endpos;
    }
};

// 使用Trie构建GSAM（另一种方法）
struct Trie {
    int son[MAXN][CHARSET];
    int fa[MAXN];
    int cnt;

    void init() {
        cnt = 0;
        memset(son[0], 0, sizeof(son[0]));
    }

    void insert(const string& s) {
        int now = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!son[now][c]) {
                son[now][c] = ++cnt;
                fa[cnt] = now;
                memset(son[cnt], 0, sizeof(son[cnt]));
            }
            now = son[now][c];
        }
    }
};

struct GSAM_from_Trie {
    Trie trie;
    GSAM gsam;
    int pos[MAXN];  // Trie节点对应的GSAM状态

    void init() {
        trie.init();
        gsam.init();
    }

    void add_string(const string& s) { trie.insert(s); }

    void build() {
        queue<pair<int, int>> q;  // {字符, Trie节点}

        for (int c = 0; c < CHARSET; c++) {
            if (trie.son[0][c]) {
                q.push({c, trie.son[0][c]});
            }
        }

        pos[0] = 0;

        while (!q.empty()) {
            auto [c, u] = q.front();
            q.pop();

            pos[u] = gsam.insert(c, pos[trie.fa[u]]);

            for (int i = 0; i < CHARSET; i++) {
                if (trie.son[u][i]) {
                    q.push({i, trie.son[u][i]});
                }
            }
        }
    }

    long long count_substrings() { return gsam.count_substrings(); }
};

// 使用示例1：直接插入字符串
void example1() {
    int n;
    cin >> n;

    GSAM gsam;
    gsam.init();

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int last = 0;
        for (char ch : s) {
            last = gsam.insert(ch - 'a', last);
        }
    }

    cout << gsam.count_substrings() << "\n";
}

// 使用示例2：使用Trie构建
void example2() {
    int n;
    cin >> n;

    GSAM_from_Trie gsam_trie;
    gsam_trie.init();

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        gsam_trie.add_string(s);
    }

    gsam_trie.build();

    cout << gsam_trie.count_substrings() << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    example1();
    // example2();

    return 0;
}
