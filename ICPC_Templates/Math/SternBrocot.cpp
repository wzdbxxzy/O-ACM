// Stern-Brocot Tree & Calkin-Wilf Tree (Stern-Brocot树)
// 功能：既约分数的二分查找、分数迭代
// 复杂度：O(log(a+b))
// 应用：分数相关问题、最优近似分数

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll gcd(ll a, ll b) { return b == 0 ? a : gcd(b, a % b); }

struct Fraction {
    ll p, q;  // p/q，保证既约

    Fraction(ll p_ = 0, ll q_ = 1) {
        ll g = gcd(abs(p_), abs(q_));
        p = p_ / g;
        q = q_ / g;
        if (q < 0) {
            p = -p;
            q = -q;
        }
    }

    bool operator<(const Fraction& other) const {
        return (__int128)p * other.q < (__int128)other.p * q;
    }

    bool operator==(const Fraction& other) const {
        return p == other.p && q == other.q;
    }

    Fraction operator+(const Fraction& other) const {
        return Fraction(p * other.q + other.p * q, q * other.q);
    }

    double value() const { return (double)p / q; }
};

// Stern-Brocot树：所有正既约分数恰好出现一次
// 树的构造：从1/1开始，每个节点(p/q)的左右儿子分别是其左右邻居的mediant
class SternBrocotTree {
   public:
    // 在[l, r]区间内二分查找分数p/q
    // 返回从根到目标的路径（'L'表示左，'R'表示右）
    string find_path(ll p, ll q) {
        Fraction target(p, q);
        Fraction left(0, 1), right(1, 0);  // 虚拟边界
        string path;

        while (true) {
            Fraction mid(left.p + right.p, left.q + right.q);  // mediant

            if (mid == target) {
                return path;
            } else if (target < mid) {
                path += 'L';
                right = mid;
            } else {
                path += 'R';
                left = mid;
            }
        }
    }

    // 根据路径找到分数
    Fraction path_to_fraction(const string& path) {
        Fraction left(0, 1), right(1, 0);

        for (char c : path) {
            Fraction mid(left.p + right.p, left.q + right.q);

            if (c == 'L') {
                right = mid;
            } else {
                left = mid;
            }
        }

        return Fraction(left.p + right.p, left.q + right.q);
    }

    // 找到小于等于p/q的最大既约分数，分母不超过max_q
    Fraction best_approximation(ll p, ll q, ll max_q) {
        Fraction target(p, q);
        Fraction left(0, 1), right(1, 1);
        Fraction best(0, 1);

        while (right.q <= max_q) {
            Fraction mid(left.p + right.p, left.q + right.q);

            if (mid.q > max_q) break;

            if (mid < target || mid == target) {
                best = mid;
                left = mid;
            } else {
                right = mid;
            }
        }

        return best;
    }

    // 计算从a/b到c/d需要多少步
    ll distance(ll a, ll b, ll c, ll d) {
        string path1 = find_path(a, b);
        string path2 = find_path(c, d);

        // 找到最长公共前缀
        int lcp = 0;
        while (lcp < (int)min(path1.size(), path2.size()) &&
               path1[lcp] == path2[lcp]) {
            lcp++;
        }

        return (path1.size() - lcp) + (path2.size() - lcp);
    }
};

// Calkin-Wilf树：另一种组织方式，遍历顺序不同
class CalkinWilfTree {
   public:
    // 第n个分数（从0开始）
    Fraction nth_fraction(ll n) {
        if (n == 0) return Fraction(1, 1);

        // 找到路径
        vector<int> path;
        ll cur = n + 1;

        while (cur > 1) {
            path.push_back(cur % 2);
            cur /= 2;
        }

        reverse(path.begin(), path.end());

        // 根据路径计算分数
        ll p = 1, q = 1;
        for (int dir : path) {
            if (dir == 0) {
                q += p;
            } else {
                p += q;
            }
        }

        return Fraction(p, q);
    }

    // 分数p/q是第几个（从0开始）
    ll fraction_to_index(ll p, ll q) {
        ll idx = 0;
        ll power = 1;

        while (p != q) {
            if (p > q) {
                idx += power;
                p -= q;
                power *= 2;
            } else {
                q -= p;
                power *= 2;
            }
        }

        return idx;
    }
};

// 应用1：连分数展开
vector<ll> continued_fraction(ll p, ll q) {
    vector<ll> cf;

    while (q != 0) {
        cf.push_back(p / q);
        ll tmp = p % q;
        p = q;
        q = tmp;
    }

    return cf;
}

// 从连分数还原分数
Fraction from_continued_fraction(const vector<ll>& cf) {
    if (cf.empty()) return Fraction(0, 1);

    ll p = 1, q = cf.back();

    for (int i = (int)cf.size() - 2; i >= 0; i--) {
        swap(p, q);
        p += cf[i] * q;
    }

    return Fraction(p, q);
}

// 应用2：最优近似
// 给定实数x和上界max_q，找分母不超过max_q的最佳近似
Fraction best_rational_approximation(double x, ll max_q) {
    ll a = 0, b = 1;  // 下界
    ll c = 1, d = 0;  // 上界

    while (b + d <= max_q) {
        ll m = a + c;
        ll n = b + d;

        double mid = (double)m / n;

        if (abs(mid - x) < 1e-9) {
            return Fraction(m, n);
        } else if (mid < x) {
            a = m;
            b = n;
        } else {
            c = m;
            d = n;
        }
    }

    // 检查a/b和c/d哪个更接近
    if (abs((double)a / b - x) < abs((double)c / d - x)) {
        return Fraction(a, b);
    } else {
        return Fraction(c, d);
    }
}

// 应用3：Farey数列
// F_n: 所有分母不超过n的既约真分数，按升序排列
vector<Fraction> farey_sequence(int n) {
    vector<Fraction> result;
    result.push_back(Fraction(0, 1));

    ll a = 0, b = 1, c = 1, d = n;
    result.push_back(Fraction(c, d));

    while (c < n) {
        ll k = (n + b) / d;
        ll tmp_a = a, tmp_b = b;
        a = c;
        b = d;
        c = k * c - tmp_a;
        d = k * d - tmp_b;

        result.push_back(Fraction(c, d));
    }

    return result;
}

// 应用4：给定分数a/b，找相邻的两个分数（Farey邻居）
pair<Fraction, Fraction> farey_neighbors(ll a, ll b, ll max_denom) {
    // 左邻居
    ll p1 = 0, q1 = 1;
    ll p2 = a, q2 = b;

    while (true) {
        ll p = p1 + p2;
        ll q = q1 + q2;

        if (q > max_denom) break;

        p1 = p;
        q1 = q;
    }

    // 右邻居
    ll p3 = 1, q3 = 0;
    ll p4 = a, q4 = b;

    while (true) {
        ll p = p3 + p4;
        ll q = q3 + q4;

        if (q > max_denom) break;

        p3 = p;
        q3 = q;
    }

    return {Fraction(p1, q1), Fraction(p3, q3)};
}

void solve() {
    SternBrocotTree sbt;

    // 示例：找到3/5在Stern-Brocot树中的路径
    string path = sbt.find_path(3, 5);
    cout << "Path to 3/5: " << path << "\n";

    // 从路径还原分数
    Fraction f = sbt.path_to_fraction(path);
    cout << "Fraction from path: " << f.p << "/" << f.q << "\n";

    // 最佳近似
    Fraction approx = best_rational_approximation(3.14159265, 100);
    cout << "Best approximation of π with denominator ≤ 100: " << approx.p
         << "/" << approx.q << " = " << approx.value() << "\n";

    // 连分数展开
    auto cf = continued_fraction(355, 113);
    cout << "Continued fraction of 355/113: ";
    for (ll x : cf) {
        cout << x << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
