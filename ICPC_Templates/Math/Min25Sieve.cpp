// Min_25 Sieve (Min_25筛)
// 功能：快速计算积性函数的前缀和
// 复杂度：O(n^(3/4) / log n)
// 应用：大范围素数求和、积性函数求和

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

const ll MOD = 1e9 + 7;

struct Min25Sieve {
    ll n, sq;
    vector<ll> primes;
    vector<ll> sp, sp2;  // 素数和、素数平方和
    vector<ll> g, g2;    // g[i] = sum of p for p <= i (p is prime)
    vector<ll> w;        // 离散化的位置

    Min25Sieve(ll n_) : n(n_) {
        sq = sqrtl(n);
        sieve();
    }

    void sieve() {
        // 线性筛预处理素数
        vector<bool> is_prime(sq + 1, true);
        is_prime[0] = is_prime[1] = false;

        sp.push_back(0);
        sp2.push_back(0);

        for (ll i = 2; i <= sq; i++) {
            if (is_prime[i]) {
                primes.push_back(i);
                sp.push_back(sp.back() + i);
                sp2.push_back(sp2.back() + i * i);

                for (ll j = i * i; j <= sq; j += i) {
                    is_prime[j] = false;
                }
            }
        }

        // 离散化
        for (ll i = 1, j; i <= n; i = j + 1) {
            j = n / (n / i);
            w.push_back(n / i);
        }

        int m = w.size();
        g.resize(m);
        g2.resize(m);

        // 初始化 g
        for (int i = 0; i < m; i++) {
            ll val = w[i];
            g[i] =
                (val % MOD) * ((val + 1) % MOD) % MOD * ((MOD + 1) / 2) % MOD -
                1;
            g[i] = (g[i] % MOD + MOD) % MOD;

            g2[i] = (val % MOD) * ((val + 1) % MOD) % MOD *
                        ((2 * val + 1) % MOD) % MOD * ((MOD + 1) / 6) % MOD -
                    1;
            g2[i] = (g2[i] % MOD + MOD) % MOD;
        }

        // Min_25筛主过程
        for (int i = 0; i < (int)primes.size(); i++) {
            ll p = primes[i];
            ll p2 = p * p;

            if (p2 > n) break;

            for (int j = 0; j < m && w[j] >= p2; j++) {
                ll val = w[j];
                int k = get_id(val / p);

                g[j] = (g[j] - p * (g[k] - sp[i] + MOD) % MOD + MOD) % MOD;
                g2[j] =
                    (g2[j] - p * p % MOD * (g2[k] - sp2[i] + MOD) % MOD + MOD) %
                    MOD;
            }
        }
    }

    int get_id(ll x) {
        if (x <= sq)
            return lower_bound(w.begin(), w.end(), x, greater<ll>()) -
                   w.begin();
        return upper_bound(w.begin(), w.end(), n / x) - w.begin() - 1;
    }

    // 计算 sum_{p <= n, p is prime} p^k
    ll prime_sum(ll k) {
        if (k == 1) return g[get_id(n)];
        if (k == 2) return g2[get_id(n)];
        return 0;  // 需要扩展
    }

    // 计算 sum_{i=1}^{n} f(i)，其中 f 是积性函数
    // 需要提供 f 在素数幂处的值
    ll S(ll n, int j) {
        if (n == 0 || primes[j] > n) return 0;

        ll ans = (g[get_id(n)] - sp[j] + MOD) % MOD;

        for (int i = j; i < (int)primes.size() && primes[i] * primes[i] <= n;
             i++) {
            ll p = primes[i];
            ll pk = p;

            for (int e = 1; pk <= n; e++, pk *= p) {
                ll f_pe = pk % MOD;  // f(p^e)，根据具体函数修改

                ans = (ans + f_pe * (S(n / pk, i + 1) + (e > 1 ? 1 : 0))) % MOD;

                if ((i128)pk * p > n) break;
            }
        }

        return ans;
    }

    ll solve() {
        if (n == 1) return 0;
        return (S(n, 0) + 1) % MOD;  // +1 是因为要算上 f(1) = 1
    }
};

// 应用1：计算前n个素数的和
ll sum_of_primes(ll n) {
    Min25Sieve sieve(n);
    return sieve.prime_sum(1);
}

// 应用2：计算 sum_{i=1}^{n} phi(i)
// phi是欧拉函数
ll sum_of_euler_phi(ll n) {
    Min25Sieve sieve(n);
    // phi(p^k) = p^(k-1) * (p-1)
    // 需要修改 S 函数
    return sieve.solve();
}

// 应用3：计算 sum_{i=1}^{n} mu(i)
// mu是莫比乌斯函数
ll sum_of_mobius(ll n) {
    // mu(p) = -1 for prime p
    // mu(p^k) = 0 for k >= 2

    Min25Sieve sieve(n);

    function<ll(ll, int)> S = [&](ll n, int j) -> ll {
        if (n == 0 || j >= (int)sieve.primes.size() || sieve.primes[j] > n) {
            return 0;
        }

        // mu(p) = -1
        ll ans = -(sieve.g[sieve.get_id(n)] - sieve.sp[j] + MOD) % MOD;
        ans = (ans % MOD + MOD) % MOD;

        for (int i = j; i < (int)sieve.primes.size() &&
                        sieve.primes[i] * sieve.primes[i] <= n;
             i++) {
            ll p = sieve.primes[i];
            // mu(p^k) = 0 for k >= 2, so only consider p^1
            ans = (ans - S(n / p, i + 1)) % MOD;
            ans = (ans % MOD + MOD) % MOD;
        }

        return ans;
    };

    return (S(n, 0) + 1) % MOD;  // +1 for mu(1) = 1
}

// 应用4：计算 sum_{i=1}^{n} sigma(i)
// sigma(i) 是 i 的约数和
ll sum_of_divisor_sum(ll n) {
    // sigma(p^k) = (p^(k+1) - 1) / (p - 1)

    Min25Sieve sieve(n);

    function<ll(ll, int)> S = [&](ll n, int j) -> ll {
        if (n == 0 || j >= (int)sieve.primes.size() || sieve.primes[j] > n) {
            return 0;
        }

        ll ans = sieve.g[sieve.get_id(n)] +
                 sieve.g[sieve.get_id(n)];  // sigma(p) = p + 1
        ans = (ans - sieve.sp[j] * 2 % MOD + MOD) % MOD;

        for (int i = j; i < (int)sieve.primes.size() &&
                        sieve.primes[i] * sieve.primes[i] <= n;
             i++) {
            ll p = sieve.primes[i];
            ll pk = p;

            for (int e = 1; pk <= n; e++, pk *= p) {
                ll sigma_pe = (power(p, e + 1) - 1 + MOD) % MOD *
                              power(p - 1, MOD - 2) % MOD;

                ans = (ans + sigma_pe * (S(n / pk, i + 1) + (e > 1 ? 1 : 0))) %
                      MOD;

                if ((i128)pk * p > n) break;
            }
        }

        return ans;
    };

    return (S(n, 0) + 1) % MOD;
}

ll power(ll a, ll b, ll p = MOD) {
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}

// 使用示例
void solve() {
    ll n;
    cin >> n;

    cout << "Sum of primes up to " << n << ": " << sum_of_primes(n) << "\n";
    cout << "Sum of Euler phi up to " << n << ": " << sum_of_euler_phi(n)
         << "\n";
    cout << "Sum of Mobius up to " << n << ": " << sum_of_mobius(n) << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
