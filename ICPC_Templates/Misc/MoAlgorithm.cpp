/**
 * 莫队算法 (Mo's Algorithm)
 * 功能：离线处理区间查询问题
 * 复杂度：O(n√n)
 */
#include <bits/stdc++.h>
using namespace std;

struct Query {
    int l, r, id;
    int block;

    bool operator<(const Query& other) const {
        if (block != other.block) {
            return block < other.block;
        }
        return (block & 1) ? (r < other.r) : (r > other.r);
    }
};

struct Mo {
    int n, m;
    int blockSize;
    vector<Query> queries;
    vector<int> cnt;
    int curAns;

    Mo(int n, int m) : n(n), m(m) {
        blockSize = sqrt(n) + 1;
        cnt.resize(n + 1);
        curAns = 0;
    }

    void addQuery(int l, int r, int id) {
        queries.push_back({l, r, id, l / blockSize});
    }

    void add(int pos, int val) {
        // 添加位置pos的元素
        if (cnt[val] == 0) {
            curAns++;
        }
        cnt[val]++;
    }

    void remove(int pos, int val) {
        // 移除位置pos的元素
        cnt[val]--;
        if (cnt[val] == 0) {
            curAns--;
        }
    }

    vector<int> solve(const vector<int>& a) {
        sort(queries.begin(), queries.end());
        vector<int> ans(m);

        int l = 0, r = -1;
        for (auto& q : queries) {
            while (r < q.r) {
                r++;
                add(r, a[r]);
            }
            while (r > q.r) {
                remove(r, a[r]);
                r--;
            }
            while (l < q.l) {
                remove(l, a[l]);
                l++;
            }
            while (l > q.l) {
                l--;
                add(l, a[l]);
            }
            ans[q.id] = curAns;
        }
        return ans;
    }
};
