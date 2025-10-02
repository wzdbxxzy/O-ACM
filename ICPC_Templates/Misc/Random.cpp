/**
 * 随机数生成器
 * 使用mt19937生成高质量随机数
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 随机数生成器
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());

// 生成 [l, r] 范围内的随机整数
ll randint(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng64); }

// 生成 [0, 1) 范围内的随机实数
double randdouble() { return uniform_real_distribution<double>(0, 1)(rng); }

// 随机打乱数组
template <typename T>
void shuffle(vector<T>& a) {
    shuffle(a.begin(), a.end(), rng);
}

// 随机选择k个不重复的数
vector<int> sample(int n, int k) {
    vector<int> res;
    set<int> used;
    while (res.size() < k) {
        int x = randint(0, n - 1);
        if (!used.count(x)) {
            used.insert(x);
            res.push_back(x);
        }
    }
    return res;
}
