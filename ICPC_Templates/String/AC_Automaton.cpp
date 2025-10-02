/**
 * AC自动机 (Aho-Corasick Automaton)
 * 功能：多模式串匹配
 * 复杂度：O(Σ|S| + n) 建树和匹配
 */
#include <bits/stdc++.h>
using namespace std;

struct AC {
    static constexpr int CHARSET = 26;
    vector<array<int, CHARSET>> tr;
    vector<int> fail, cnt;
    int idx;

    AC(int maxn = 1e6) {
        tr.resize(maxn);
        fail.resize(maxn);
        cnt.resize(maxn);
        idx = 0;
    }

    void insert(string s) {
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!tr[p][u]) {
                tr[p][u] = ++idx;
            }
            p = tr[p][u];
        }
        cnt[p]++;
    }

    void build() {
        queue<int> q;
        for (int i = 0; i < CHARSET; i++) {
            if (tr[0][i]) {
                q.push(tr[0][i]);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < CHARSET; i++) {
                if (tr[u][i]) {
                    fail[tr[u][i]] = tr[fail[u]][i];
                    q.push(tr[u][i]);
                } else {
                    tr[u][i] = tr[fail[u]][i];
                }
            }
        }
    }

    int query(string s) {
        int p = 0, res = 0;
        for (char c : s) {
            int u = c - 'a';
            p = tr[p][u];
            for (int j = p; j && ~cnt[j]; j = fail[j]) {
                res += cnt[j];
                cnt[j] = -1;  // 避免重复计数
            }
        }
        return res;
    }
};
