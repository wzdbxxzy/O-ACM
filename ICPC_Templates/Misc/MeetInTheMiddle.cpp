// Meet in the Middle (折半搜索/双向搜索)
// 功能：将搜索空间一分为二，降低复杂度
// 复杂度：O(2^(n/2))，相比暴力的O(2^n)大幅优化
// 应用：子集和问题、背包问题、组合优化

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ========== 基础框架 ==========

// 1. 子集和问题：从n个数中选若干个，使和最接近target
ll closest_subset_sum(const vector<ll>& a, ll target) {
    int n = a.size();
    int half = n / 2;

    // 枚举前半部分的所有子集和
    vector<ll> left_sums;
    for (int mask = 0; mask < (1 << half); mask++) {
        ll sum = 0;
        for (int i = 0; i < half; i++) {
            if (mask >> i & 1) {
                sum += a[i];
            }
        }
        left_sums.push_back(sum);
    }

    sort(left_sums.begin(), left_sums.end());

    // 枚举后半部分，在前半部分中二分查找
    ll ans = LLONG_MAX;
    for (int mask = 0; mask < (1 << (n - half)); mask++) {
        ll sum = 0;
        for (int i = 0; i < n - half; i++) {
            if (mask >> i & 1) {
                sum += a[half + i];
            }
        }

        // 在left_sums中找最接近target-sum的值
        ll need = target - sum;

        // lower_bound
        auto it = lower_bound(left_sums.begin(), left_sums.end(), need);
        if (it != left_sums.end()) {
            ans = min(ans, abs(*it + sum - target));
        }
        if (it != left_sums.begin()) {
            --it;
            ans = min(ans, abs(*it + sum - target));
        }
    }

    return ans;
}

// 2. 子集和等于target的方案数
ll count_subset_sum(const vector<ll>& a, ll target) {
    int n = a.size();
    int half = n / 2;

    // 前半部分：记录每个和出现的次数
    map<ll, ll> left_count;
    for (int mask = 0; mask < (1 << half); mask++) {
        ll sum = 0;
        for (int i = 0; i < half; i++) {
            if (mask >> i & 1) {
                sum += a[i];
            }
        }
        left_count[sum]++;
    }

    // 后半部分：查询前半部分
    ll ans = 0;
    for (int mask = 0; mask < (1 << (n - half)); mask++) {
        ll sum = 0;
        for (int i = 0; i < n - half; i++) {
            if (mask >> i & 1) {
                sum += a[half + i];
            }
        }

        ll need = target - sum;
        if (left_count.count(need)) {
            ans += left_count[need];
        }
    }

    return ans;
}

// 3. 4-SUM问题：从4个数组中各选一个数，使和为0
ll four_sum_count(const vector<int>& A, const vector<int>& B,
                  const vector<int>& C, const vector<int>& D) {
    map<int, int> ab_sum;

    // 枚举A和B的所有组合
    for (int a : A) {
        for (int b : B) {
            ab_sum[a + b]++;
        }
    }

    // 枚举C和D，查询A+B中是否有-(C+D)
    ll ans = 0;
    for (int c : C) {
        for (int d : D) {
            int need = -(c + d);
            if (ab_sum.count(need)) {
                ans += ab_sum[need];
            }
        }
    }

    return ans;
}

// 4. 最大异或和子集：选若干个数异或，使结果最大
ll max_xor_subset(const vector<ll>& a, ll target) {
    int n = a.size();
    int half = n / 2;

    // 前半部分所有异或值
    set<ll> left_xor;
    for (int mask = 0; mask < (1 << half); mask++) {
        ll xor_val = 0;
        for (int i = 0; i < half; i++) {
            if (mask >> i & 1) {
                xor_val ^= a[i];
            }
        }
        left_xor.insert(xor_val);
    }

    // 后半部分枚举
    ll ans = 0;
    for (int mask = 0; mask < (1 << (n - half)); mask++) {
        ll xor_val = 0;
        for (int i = 0; i < n - half; i++) {
            if (mask >> i & 1) {
                xor_val ^= a[half + i];
            }
        }

        // 在前半部分中找使异或和最大的值
        // 贪心：从高位到低位尝试
        ll best = 0;
        for (ll x : left_xor) {
            best = max(best, x ^ xor_val);
        }
        ans = max(ans, best);
    }

    return ans;
}

// ========== 经典应用 ==========

// 应用1：0-1背包的方案数
// 容量为W，n个物品，求装满的方案数
ll knapsack_count(const vector<int>& w, int W) {
    int n = w.size();
    int half = n / 2;

    // 前半部分：dp[i]表示重量为i的方案数
    map<int, ll> left_dp;
    left_dp[0] = 1;

    for (int i = 0; i < half; i++) {
        map<int, ll> new_dp = left_dp;
        for (auto [weight, cnt] : left_dp) {
            if (weight + w[i] <= W) {
                new_dp[weight + w[i]] += cnt;
            }
        }
        left_dp = new_dp;
    }

    // 后半部分
    ll ans = 0;
    map<int, ll> right_dp;
    right_dp[0] = 1;

    for (int i = half; i < n; i++) {
        map<int, ll> new_dp = right_dp;
        for (auto [weight, cnt] : right_dp) {
            if (weight + w[i] <= W) {
                new_dp[weight + w[i]] += cnt;
            }
        }
        right_dp = new_dp;
    }

    // 合并
    for (auto [w1, cnt1] : left_dp) {
        int need = W - w1;
        if (right_dp.count(need)) {
            ans += cnt1 * right_dp[need];
        }
    }

    return ans;
}

// 应用2：选数使乘积为完全平方数
// 给定n个数，选若干个使乘积为完全平方数，求方案数
ll perfect_square_product(vector<ll> a) {
    int n = a.size();

    // 分解质因数，只保留指数的奇偶性
    vector<map<int, int>> factors(n);
    for (int i = 0; i < n; i++) {
        ll x = a[i];
        for (ll p = 2; p * p <= x; p++) {
            while (x % p == 0) {
                factors[i][p] ^= 1;
                x /= p;
            }
        }
        if (x > 1) {
            factors[i][x] ^= 1;
        }
    }

    // Meet in the Middle
    int half = n / 2;
    map<map<int, int>, ll> left_states;

    for (int mask = 0; mask < (1 << half); mask++) {
        map<int, int> state;
        for (int i = 0; i < half; i++) {
            if (mask >> i & 1) {
                for (auto [p, cnt] : factors[i]) {
                    state[p] ^= cnt;
                }
            }
        }
        left_states[state]++;
    }

    ll ans = 0;
    for (int mask = 0; mask < (1 << (n - half)); mask++) {
        map<int, int> state;
        for (int i = 0; i < n - half; i++) {
            if (mask >> i & 1) {
                for (auto [p, cnt] : factors[half + i]) {
                    state[p] ^= cnt;
                }
            }
        }

        // 需要前半部分的状态与当前状态异或后全为0
        if (left_states.count(state)) {
            ans += left_states[state];
        }
    }

    return ans;
}

// 应用3：最小化最大值问题
// 将n个数分成两组，最小化两组和的最大值
ll min_max_subset_sum(const vector<ll>& a) {
    int n = a.size();
    ll total = accumulate(a.begin(), a.end(), 0LL);

    // 找最接近total/2的子集和
    ll target = total / 2;

    int half = n / 2;
    vector<ll> left_sums;

    for (int mask = 0; mask < (1 << half); mask++) {
        ll sum = 0;
        for (int i = 0; i < half; i++) {
            if (mask >> i & 1) {
                sum += a[i];
            }
        }
        if (sum <= target) {
            left_sums.push_back(sum);
        }
    }

    sort(left_sums.begin(), left_sums.end());

    ll best_sum = 0;
    for (int mask = 0; mask < (1 << (n - half)); mask++) {
        ll sum = 0;
        for (int i = 0; i < n - half; i++) {
            if (mask >> i & 1) {
                sum += a[half + i];
            }
        }

        if (sum <= target) {
            ll need = target - sum;
            auto it = upper_bound(left_sums.begin(), left_sums.end(), need);
            if (it != left_sums.begin()) {
                --it;
                best_sum = max(best_sum, *it + sum);
            }
        }
    }

    return max(best_sum, total - best_sum);
}

// 使用示例
void solve() {
    int n;
    ll target;
    cin >> n >> target;

    vector<ll> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // 子集和等于target的方案数
    ll ans = count_subset_sum(a, target);
    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
