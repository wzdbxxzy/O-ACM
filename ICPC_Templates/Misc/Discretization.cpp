/**
 * 离散化 (Discretization)
 * 功能：将大范围数据映射到小范围
 * 复杂度：O(n log n)
 */
#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Discrete {
    vector<T> xs;

    Discrete(const vector<T>& v) {
        xs = v;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
    }

    int get(const T& x) const {
        return lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    }

    T operator[](int i) const { return xs[i]; }

    int size() const { return xs.size(); }
};

// 使用示例
void example() {
    vector<int> a = {100, 5, 200, 5, 300};
    Discrete<int> disc(a);

    // disc.get(5) = 0
    // disc.get(100) = 1
    // disc.get(200) = 2
    // disc.get(300) = 3

    // disc[0] = 5
    // disc[1] = 100
    // disc[2] = 200
    // disc[3] = 300
}
