/**
 * ST表 (Sparse Table)
 * 功能：静态区间RMQ查询
 * 复杂度：预处理 O(n log n)，查询 O(1)
 */
#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> st;
    int n;

    SparseTable() {}
    SparseTable(const vector<T>& a) {
        n = a.size();
        int lg = __lg(n) + 1;
        st.assign(lg, vector<T>(n));
        st[0] = a;

        for (int i = 1; i < lg; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    T query(int l, int r) {  // [l, r)
        int k = __lg(r - l);
        return min(st[k][l], st[k][r - (1 << k)]);
    }
};
