// Ternary Search (三分查找)
// 功能：在单峰/单谷函数上找最值
// 复杂度：O(log n)
// 应用：凸函数/凹函数最优化

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

// 实数域三分
// 找f(x)的最小值
double ternary_search_min(function<double(double)> f, double l, double r) {
    while (r - l > EPS) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;

        if (f(m1) > f(m2)) {
            l = m1;
        } else {
            r = m2;
        }
    }

    return (l + r) / 2;
}

// 找f(x)的最大值
double ternary_search_max(function<double(double)> f, double l, double r) {
    while (r - l > EPS) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;

        if (f(m1) < f(m2)) {
            l = m1;
        } else {
            r = m2;
        }
    }

    return (l + r) / 2;
}

// 整数域三分（最小值）
ll ternary_search_int_min(function<ll(ll)> f, ll l, ll r) {
    while (r - l > 2) {
        ll m1 = l + (r - l) / 3;
        ll m2 = r - (r - l) / 3;

        if (f(m1) > f(m2)) {
            l = m1;
        } else {
            r = m2;
        }
    }

    ll ans = l;
    for (ll i = l; i <= r; i++) {
        if (f(i) < f(ans)) {
            ans = i;
        }
    }

    return ans;
}

// 应用1：最小化最大距离
// 给定n个点，在[0, L]上选一个点，最小化到所有点的最大距离
double min_max_distance(const vector<double>& points, double L) {
    auto f = [&](double x) {
        double max_dist = 0;
        for (double p : points) {
            max_dist = max(max_dist, abs(p - x));
        }
        return max_dist;
    };

    double pos = ternary_search_min(f, 0, L);
    return f(pos);
}

// 应用2：抛物线问题
// 求抛物线 y = ax^2 + bx + c 在[l, r]上的最值
pair<double, double> parabola_range(double a, double b, double c, double l,
                                    double r) {
    auto f = [&](double x) { return a * x * x + b * x + c; };

    if (a > 0) {
        // 开口向上，求最小值
        double x_min = -b / (2 * a);
        if (x_min < l) x_min = l;
        if (x_min > r) x_min = r;

        double y_min = f(x_min);
        double y_max = max(f(l), f(r));

        return {y_min, y_max};
    } else {
        // 开口向下，求最大值
        double x_max = -b / (2 * a);
        if (x_max < l) x_max = l;
        if (x_max > r) x_max = r;

        double y_max = f(x_max);
        double y_min = min(f(l), f(r));

        return {y_min, y_max};
    }
}

// 应用3：最优分割点
// 将数组分成两部分，最小化两部分和的差的平方
ll optimal_split(const vector<ll>& a) {
    int n = a.size();
    vector<ll> prefix(n + 1, 0);

    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + a[i];
    }

    ll total = prefix[n];

    auto f = [&](ll k) {
        ll left_sum = prefix[k];
        ll right_sum = total - left_sum;
        return (left_sum - right_sum) * (left_sum - right_sum);
    };

    ll k = ternary_search_int_min(f, 1, n - 1);
    return k;
}

// 应用4：两点间最短路径（绕过障碍）
// 从A到B，但要绕过区间[l, r]上的障碍
struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    double dist(const Point& p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
};

double shortest_path_with_obstacle(Point A, Point B, double l, double r,
                                   double h) {
    // 障碍是y=h的线段[l, r]
    // 如果不相交，直接距离
    if (A.y <= h && B.y <= h) return A.dist(B);
    if (A.y >= h && B.y >= h) return A.dist(B);

    // 需要绕过，三分找最优绕行点
    auto f = [&](double x) {
        Point P(x, h);
        return A.dist(P) + P.dist(B);
    };

    double x = ternary_search_min(f, l, r);
    return f(x);
}

// 应用5：凸函数最优化（离散点）
// 给定n个点，找一个点使得到所有点的距离平方和最小
Point find_centroid(const vector<Point>& points) {
    auto f = [&](double x, double y) {
        double sum = 0;
        for (const auto& p : points) {
            double dx = p.x - x;
            double dy = p.y - y;
            sum += dx * dx + dy * dy;
        }
        return sum;
    };

    // 先对x三分
    double min_x = 1e9, max_x = -1e9;
    for (const auto& p : points) {
        min_x = min(min_x, p.x);
        max_x = max(max_x, p.x);
    }

    auto fx = [&](double x) {
        // 对于固定的x，再对y三分
        double min_y = 1e9, max_y = -1e9;
        for (const auto& p : points) {
            min_y = min(min_y, p.y);
            max_y = max(max_y, p.y);
        }

        return ternary_search_min([&](double y) { return f(x, y); }, min_y,
                                  max_y);
    };

    double best_x =
        ternary_search_min([&](double x) { return fx(x); }, min_x, max_x);

    double min_y = 1e9, max_y = -1e9;
    for (const auto& p : points) {
        min_y = min(min_y, p.y);
        max_y = max(max_y, p.y);
    }

    double best_y = ternary_search_min([&](double y) { return f(best_x, y); },
                                       min_y, max_y);

    return Point(best_x, best_y);
}

// 应用6：最优速度分配
// 总路程为D，分成n段，第i段长度di，速度vi有范围[li, ri]
// 最小化总时间，满足平均速度至少为v_avg
double optimal_speed_allocation() {
    int n;
    double D, v_avg;
    cin >> n >> D >> v_avg;

    vector<double> d(n), l(n), r(n);
    for (int i = 0; i < n; i++) {
        cin >> d[i] >> l[i] >> r[i];
    }

    double max_time = D / v_avg;

    // 对于每一段，三分找最优速度
    // 这里简化：假设每段独立优化
    double total_time = 0;
    for (int i = 0; i < n; i++) {
        // 在[li, ri]范围内，时间 t = d[i] / v
        // v越大，t越小
        // 如果有其他约束，可以用三分
        double v_opt = r[i];  // 选最大速度
        total_time += d[i] / v_opt;
    }

    return total_time;
}

void solve() {
    // 示例：找f(x) = (x-3)^2 + 5在[0, 10]上的最小值
    auto f = [](double x) { return (x - 3) * (x - 3) + 5; };

    double x_min = ternary_search_min(f, 0, 10);
    cout << fixed << setprecision(10);
    cout << "Minimum at x = " << x_min << "\n";
    cout << "f(x) = " << f(x_min) << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
