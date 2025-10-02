/**
 * 比赛常用模板头文件
 * 包含常用宏定义、类型定义和实用函数
 */
#include <bits/stdc++.h>
using namespace std;

// 类型定义
using ll = long long;
using ull = unsigned long long;
using i64 = long long;
using u64 = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

// 常用宏
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define mp make_pair
#define fi first
#define se second

// 输入输出优化
void fastio() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

// debug输出
template <typename T>
void debug(const T& x) {
    cerr << x << endl;
}

template <typename T, typename... Args>
void debug(const T& x, const Args&... args) {
    cerr << x << " ";
    debug(args...);
}

// 二分查找模板
template <typename T, typename F>
T binary_search_first(T l, T r, F check) {
    while (l < r) {
        T mid = l + (r - l) / 2;
        if (check(mid)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    return l;
}

template <typename T, typename F>
T binary_search_last(T l, T r, F check) {
    while (l < r) {
        T mid = l + (r - l + 1) / 2;
        if (check(mid)) {
            l = mid;
        } else {
            r = mid - 1;
        }
    }
    return l;
}

// 实数二分
double binary_search_real(double l, double r, function<bool(double)> check) {
    for (int i = 0; i < 100; i++) {
        double mid = (l + r) / 2;
        if (check(mid)) {
            r = mid;
        } else {
            l = mid;
        }
    }
    return l;
}

// 读入优化
inline int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

// 主函数模板
void solve() {
    // 在这里写解题代码
}

int main() {
    fastio();
    int T = 1;
    // cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}
