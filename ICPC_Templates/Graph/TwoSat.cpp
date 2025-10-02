/**
 * 2-SAT (Two Satisfiability)
 * 功能：判定布尔方程组是否有解
 * 复杂度：O(n + m)
 */
#include <bits/stdc++.h>
using namespace std;

struct TwoSat {
    int n;
    vector<vector<int>> e;
    vector<bool> ans;

    TwoSat(int n) : n(n), e(2 * n), ans(n) {}

    // 添加约束：(u, f) OR (v, g)
    // f, g 为 true 表示取正，false 表示取反
    void addClause(int u, bool f, int v, bool g) {
        e[2 * u + !f].push_back(2 * v + g);
        e[2 * v + !g].push_back(2 * u + f);
    }

    bool satisfiable() {
        vector<int> id(2 * n, -1), dfn(2 * n, -1), low(2 * n, -1);
        vector<int> stk;
        int now = 0, cnt = 0;

        function<void(int)> tarjan = [&](int u) {
            stk.push_back(u);
            dfn[u] = low[u] = now++;
            for (auto v : e[u]) {
                if (dfn[v] == -1) {
                    tarjan(v);
                    low[u] = min(low[u], low[v]);
                } else if (id[v] == -1) {
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (dfn[u] == low[u]) {
                int v;
                do {
                    v = stk.back();
                    stk.pop_back();
                    id[v] = cnt;
                } while (v != u);
                ++cnt;
            }
        };

        for (int i = 0; i < 2 * n; ++i) {
            if (dfn[i] == -1) {
                tarjan(i);
            }
        }

        for (int i = 0; i < n; ++i) {
            if (id[2 * i] == id[2 * i + 1]) {
                return false;
            }
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }

    vector<bool> answer() { return ans; }
};
