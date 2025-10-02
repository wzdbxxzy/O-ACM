// Baby-Step Giant-Step (BSGS / 大步小步算法)
// 功能：求解离散对数问题 a^x ≡ b (mod p)
// 复杂度：O(√p)
// 应用：离散对数、原根、高次同余方程

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll power(ll a, ll b, ll p) {
    ll res = 1;
    a %= p;
    while (b > 0) {
        if (b & 1) res = (__int128)res * a % p;
        a = (__int128)a * a % p;
        b >>= 1;
    }
    return res;
}

ll gcd(ll a, ll b) { return b == 0 ? a : gcd(b, a % b); }

// BSGS：求最小的x使得 a^x ≡ b (mod p)
// 要求 gcd(a, p) = 1
ll bsgs(ll a, ll b, ll p) {
    a %= p;
    b %= p;
    if (b == 1) return 0;

    ll m = ceil(sqrt(p));

    // Baby step: 计算 a^j mod p (j=0..m-1)
    unordered_map<ll, ll> baby;
    ll cur = 1;
    for (ll j = 0; j < m; j++) {
        if (!baby.count(cur)) {
            baby[cur] = j;
        }
        cur = (__int128)cur * a % p;
    }

    // Giant step: 计算 b * (a^m)^(-i) mod p
    ll am = power(a, m, p);
    ll inv_am = power(am, p - 2, p);  // 费马小定理求逆元

    cur = b;
    for (ll i = 0; i < m; i++) {
        if (baby.count(cur)) {
            ll ans = i * m + baby[cur];
            if (ans > 0) return ans;
        }
        cur = (__int128)cur * inv_am % p;
    }

    return -1;  // 无解
}

// 扩展BSGS：处理 gcd(a, p) != 1 的情况
ll exbsgs(ll a, ll b, ll p) {
    a %= p;
    b %= p;
    if (b == 1) return 0;

    ll cnt = 0, t = 1;

    // 先处理 gcd(a, p) != 1 的情况
    while (true) {
        ll d = gcd(a, p);
        if (d == 1) break;
        if (b % d != 0) return -1;

        cnt++;
        b /= d;
        p /= d;
        t = (__int128)t * (a / d) % p;

        if (t == b) return cnt;
    }

    // 现在 gcd(a, p) = 1，使用普通BSGS
    // 求解 a^x ≡ b * t^(-1) (mod p)
    ll inv_t = power(t, p - 2, p);
    b = (__int128)b * inv_t % p;

    ll res = bsgs(a, b, p);
    if (res == -1) return -1;
    return res + cnt;
}

// 求原根
// g是p的原根当且仅当 g^((p-1)/q) ≢ 1 (mod p) 对所有p-1的质因子q
bool is_primitive_root(ll g, ll p, const vector<ll>& prime_factors) {
    for (ll q : prime_factors) {
        if (power(g, (p - 1) / q, p) == 1) {
            return false;
        }
    }
    return true;
}

ll find_primitive_root(ll p) {
    if (p == 2) return 1;

    // 分解p-1
    ll phi = p - 1;
    vector<ll> factors;
    ll tmp = phi;

    for (ll i = 2; i * i <= tmp; i++) {
        if (tmp % i == 0) {
            factors.push_back(i);
            while (tmp % i == 0) {
                tmp /= i;
            }
        }
    }
    if (tmp > 1) factors.push_back(tmp);

    // 枚举找原根
    for (ll g = 2; g < p; g++) {
        if (is_primitive_root(g, p, factors)) {
            return g;
        }
    }

    return -1;
}

// 高次剩余（k次剩余）
// 求解 x^k ≡ a (mod p)，p为质数
ll kth_residue(ll a, ll k, ll p) {
    // 设p的原根为g
    // x = g^y, a = g^t
    // 则 (g^y)^k ≡ g^t (mod p)
    // 即 ky ≡ t (mod p-1)

    ll g = find_primitive_root(p);
    if (g == -1) return -1;

    // 求t：g^t ≡ a (mod p)
    ll t = bsgs(g, a, p);
    if (t == -1) return -1;

    // 求解 ky ≡ t (mod p-1)
    ll phi = p - 1;
    ll d = gcd(k, phi);

    if (t % d != 0) return -1;  // 无解

    // 简化方程：(k/d)y ≡ t/d (mod phi/d)
    k /= d;
    t /= d;
    phi /= d;

    // 求逆元
    ll inv_k = power(k, phi - 1, phi);  // 因为现在gcd(k, phi)=1
    ll y = (__int128)t * inv_k % phi;

    // x = g^y mod p
    return power(g, y, p);
}

// 应用：离散对数表
// 预处理小范围的离散对数
struct DiscreteLogTable {
    ll p, g;
    unordered_map<ll, ll> table;

    DiscreteLogTable(ll p_, ll g_) : p(p_), g(g_) { build(); }

    void build() {
        ll val = 1;
        for (ll i = 0; i < p; i++) {
            if (!table.count(val)) {
                table[val] = i;
            }
            val = (__int128)val * g % p;
        }
    }

    ll query(ll x) {
        x %= p;
        if (table.count(x)) {
            return table[x];
        }
        return -1;
    }
};

// 使用示例
void solve() {
    ll a, b, p;
    cin >> a >> b >> p;

    // 求 a^x ≡ b (mod p)
    ll x = exbsgs(a, b, p);

    if (x == -1) {
        cout << "No solution\n";
    } else {
        cout << "x = " << x << "\n";

        // 验证
        ll check = power(a, x, p);
        cout << "Verification: " << a << "^" << x << " ≡ " << check << " (mod "
             << p << ")\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
