/**
 * 分块 (Block Decomposition)
 * 功能：区间查询与修改
 * 复杂度：O(√n)
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct BlockDecomposition {
    int n, blockSize, blockNum;
    vector<ll> a;
    vector<ll> blockSum;
    vector<ll> blockAdd;

    BlockDecomposition(int n) : n(n) {
        blockSize = sqrt(n) + 1;
        blockNum = (n + blockSize - 1) / blockSize;
        a.resize(n);
        blockSum.resize(blockNum);
        blockAdd.resize(blockNum);
    }

    BlockDecomposition(const vector<ll>& init)
        : BlockDecomposition(init.size()) {
        a = init;
        for (int i = 0; i < n; i++) {
            blockSum[i / blockSize] += a[i];
        }
    }

    void add(int l, int r, ll val) {
        int bl = l / blockSize;
        int br = r / blockSize;

        if (bl == br) {
            for (int i = l; i <= r; i++) {
                a[i] += val;
                blockSum[bl] += val;
            }
        } else {
            // 左边不完整的块
            for (int i = l; i < (bl + 1) * blockSize; i++) {
                a[i] += val;
                blockSum[bl] += val;
            }
            // 中间完整的块
            for (int i = bl + 1; i < br; i++) {
                blockAdd[i] += val;
            }
            // 右边不完整的块
            for (int i = br * blockSize; i <= r; i++) {
                a[i] += val;
                blockSum[br] += val;
            }
        }
    }

    ll query(int l, int r) {
        int bl = l / blockSize;
        int br = r / blockSize;
        ll res = 0;

        if (bl == br) {
            for (int i = l; i <= r; i++) {
                res += a[i] + blockAdd[bl];
            }
        } else {
            // 左边不完整的块
            for (int i = l; i < (bl + 1) * blockSize; i++) {
                res += a[i] + blockAdd[bl];
            }
            // 中间完整的块
            for (int i = bl + 1; i < br; i++) {
                res += blockSum[i] + blockAdd[i] * blockSize;
            }
            // 右边不完整的块
            for (int i = br * blockSize; i <= r; i++) {
                res += a[i] + blockAdd[br];
            }
        }

        return res;
    }
};
