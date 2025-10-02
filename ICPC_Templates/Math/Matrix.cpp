/**
 * 矩阵 (Matrix)
 * 功能：矩阵乘法、快速幂
 * 应用：递推优化、图论等
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T>
struct Matrix {
    int n, m;
    vector<vector<T>> a;

    Matrix(int n = 0, int m = 0) : n(n), m(m) { a.assign(n, vector<T>(m)); }

    Matrix(vector<vector<T>> a) : n(a.size()), m(a[0].size()), a(a) {}

    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; i++) {
            res[i][i] = 1;
        }
        return res;
    }

    vector<T>& operator[](int i) { return a[i]; }

    const vector<T>& operator[](int i) const { return a[i]; }

    Matrix operator*(const Matrix& b) const {
        assert(m == b.n);
        Matrix res(n, b.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < b.m; j++) {
                for (int k = 0; k < m; k++) {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return res;
    }

    Matrix operator+(const Matrix& b) const {
        assert(n == b.n && m == b.m);
        Matrix res(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                res[i][j] = a[i][j] + b[i][j];
            }
        }
        return res;
    }

    Matrix operator-(const Matrix& b) const {
        assert(n == b.n && m == b.m);
        Matrix res(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                res[i][j] = a[i][j] - b[i][j];
            }
        }
        return res;
    }

    Matrix power(ll k) const {
        assert(n == m);
        Matrix res = identity(n);
        Matrix base = *this;
        while (k) {
            if (k & 1) res = res * base;
            base = base * base;
            k >>= 1;
        }
        return res;
    }
};

// 高斯消元
template <typename T>
int gauss(vector<vector<T>>& a) {
    int n = a.size();
    int m = a[0].size() - 1;

    vector<int> pivot(m, -1);
    int rank = 0;

    for (int col = 0, row = 0; col < m && row < n; col++) {
        int sel = row;
        for (int i = row; i < n; i++) {
            if (abs(a[i][col]) > abs(a[sel][col])) {
                sel = i;
            }
        }
        if (abs(a[sel][col]) < 1e-9) continue;

        swap(a[sel], a[row]);
        pivot[col] = row;

        for (int i = 0; i < n; i++) {
            if (i != row && abs(a[i][col]) > 1e-9) {
                T t = a[i][col] / a[row][col];
                for (int j = col; j <= m; j++) {
                    a[i][j] -= t * a[row][j];
                }
            }
        }
        row++;
        rank++;
    }

    return rank;
}
