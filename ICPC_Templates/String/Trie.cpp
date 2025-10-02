/**
 * 字典树 (Trie)
 * 功能：高效存储和检索字符串集合
 * 复杂度：O(L) 每次操作，L为字符串长度
 */
#include <bits/stdc++.h>
using namespace std;

struct Trie {
    vector<array<int, 26>> trie;
    vector<int> cnt;
    int idx;

    Trie(int maxn = 1e6) {
        trie.resize(maxn);
        cnt.resize(maxn);
        idx = 0;
    }

    void insert(string s) {
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!trie[p][u]) {
                trie[p][u] = ++idx;
            }
            p = trie[p][u];
        }
        cnt[p]++;
    }

    bool search(string s) {
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!trie[p][u]) {
                return false;
            }
            p = trie[p][u];
        }
        return cnt[p] > 0;
    }

    int count(string s) {
        int p = 0;
        for (char c : s) {
            int u = c - 'a';
            if (!trie[p][u]) {
                return 0;
            }
            p = trie[p][u];
        }
        return cnt[p];
    }
};

// 01 Trie（用于异或相关问题）
struct Trie01 {
    vector<array<int, 2>> trie;
    int idx;

    Trie01(int maxn = 1e6) {
        trie.resize(maxn);
        idx = 0;
    }

    void insert(int x) {
        int p = 0;
        for (int i = 30; i >= 0; i--) {
            int u = (x >> i) & 1;
            if (!trie[p][u]) {
                trie[p][u] = ++idx;
            }
            p = trie[p][u];
        }
    }

    int query_max(int x) {
        int p = 0, res = 0;
        for (int i = 30; i >= 0; i--) {
            int u = (x >> i) & 1;
            if (trie[p][u ^ 1]) {
                res |= (1 << i);
                p = trie[p][u ^ 1];
            } else {
                p = trie[p][u];
            }
        }
        return res;
    }
};
