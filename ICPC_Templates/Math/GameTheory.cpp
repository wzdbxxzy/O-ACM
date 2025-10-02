// Game Theory (博弈论)
// 功能：SG函数、Nim游戏、Anti-SG
// 复杂度：依具体问题而定
// 应用：公平组合游戏

#include <bits/stdc++.h>
using namespace std;

// SG函数计算
namespace SG {
const int MAXN = 1e4 + 10;
int sg[MAXN];
bool vis[MAXN];

// 根据后继状态计算SG值
// moves: 当前状态的所有合法后继状态
int calc_sg(int x, function<vector<int>(int)> get_moves) {
    if (sg[x] != -1) return sg[x];

    memset(vis, false, sizeof(vis));
    vector<int> moves = get_moves(x);

    for (int next : moves) {
        int sg_next = calc_sg(next, get_moves);
        vis[sg_next] = true;
    }

    // mex 运算
    for (int i = 0;; i++) {
        if (!vis[i]) {
            return sg[x] = i;
        }
    }
}

// 初始化
void init() { memset(sg, -1, sizeof(sg)); }
}  // namespace SG

// Nim游戏
namespace Nim {
// 基础Nim：每次可以从某一堆取任意个
// 结论：所有堆石子数异或和为0则后手必胜
bool is_winning(const vector<int>& piles) {
    int xor_sum = 0;
    for (int x : piles) {
        xor_sum ^= x;
    }
    return xor_sum != 0;  // 非0则先手必胜
}

// 阶梯Nim：奇数阶梯的石子异或和
bool stair_nim(const vector<int>& stairs) {
    int xor_sum = 0;
    for (int i = 0; i < (int)stairs.size(); i++) {
        if (i % 2 == 1) {  // 奇数阶梯
            xor_sum ^= stairs[i];
        }
    }
    return xor_sum != 0;
}

// Nim积：两个Nim值的乘积
map<pair<int, int>, int> nim_product_memo;

int nim_product(int x, int y) {
    if (x == 0 || y == 0) return 0;
    if (x == 1) return y;
    if (y == 1) return x;
    if (x > y) swap(x, y);

    auto key = make_pair(x, y);
    if (nim_product_memo.count(key)) {
        return nim_product_memo[key];
    }

    // Nim积的计算（根据具体游戏规则）
    // 这里给出一般递推形式
    int res = 0;
    // ... 具体实现依游戏而定
    return nim_product_memo[key] = res;
}
}  // namespace Nim

// Anti-SG游戏（所有后继状态SG=0时，当前状态判负）
namespace AntiSG {
// SJ定理：
// 1. 游戏的SG函数不为0且游戏中某个单一游戏的SG函数 > 1, 则先手必胜
// 2. 游戏的SG函数为0或所有单一游戏SG函数都 <= 1，则先手必败

bool is_winning(const vector<int>& sg_values) {
    int xor_sum = 0;
    bool has_large = false;

    for (int sg : sg_values) {
        xor_sum ^= sg;
        if (sg > 1) has_large = true;
    }

    if (has_large) {
        return xor_sum != 0;  // 有大于1的SG值，按普通Nim判断
    } else {
        return xor_sum == 0;  // 所有SG值≤1，反过来判断
    }
}
}  // namespace AntiSG

// ========== 经典游戏 ==========

// 1. 取石子游戏
namespace TakeStones {
// 每次可以取 [1, m] 个石子
// SG(x) = x % (m+1)
int sg_value(int n, int m) { return n % (m + 1); }

// 斐波那契博弈：每次最多取对手上次取的2倍
// 结论：n是Fibonacci数则后手必胜
bool is_fibonacci(int n) {
    int a = 1, b = 1;
    while (b < n) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b == n;
}

bool fibonacci_game(int n) {
    return !is_fibonacci(n);  // 不是Fibonacci数则先手必胜
}
}  // namespace TakeStones

// 2. 翻硬币游戏
namespace FlipCoins {
// n个硬币，每次翻转连续的k个
// SG值计算
vector<int> moves = {1, 2, 3};  // 可以翻转的硬币数

int calc_sg(const string& state) {
    // 状态压缩后计算SG值
    static map<string, int> memo;
    if (memo.count(state)) return memo[state];

    set<int> reachable;
    for (int k : moves) {
        for (int i = 0; i + k <= (int)state.size(); i++) {
            bool can_flip = true;
            for (int j = i; j < i + k; j++) {
                if (state[j] == '0') can_flip = false;
            }
            if (can_flip) {
                string next = state;
                for (int j = i; j < i + k; j++) {
                    next[j] = '0';
                }
                reachable.insert(calc_sg(next));
            }
        }
    }

    int mex = 0;
    while (reachable.count(mex)) mex++;
    return memo[state] = mex;
}
}  // namespace FlipCoins

// 3. 巴什博奕（Bash Game）
namespace BashGame {
// n个石子，每次取[1, m]个，取走最后一个石子者胜
// 结论：n%(m+1)==0 则后手必胜
bool first_win(int n, int m) { return n % (m + 1) != 0; }

// 扩展：可以取的数量是集合S
// 需要计算SG函数
}  // namespace BashGame

// 4. 威佐夫博弈（Wythoff Game）
namespace WythoffGame {
// 两堆石子，每次可以：
// 1. 从某一堆取任意个
// 2. 从两堆同时取相同数量

// 奇异局势：(ak, bk) = (k⌊kφ⌋, k⌊kφ²⌋), φ=(1+√5)/2
const double PHI = (1.0 + sqrt(5.0)) / 2.0;

bool is_losing(int a, int b) {
    if (a > b) swap(a, b);
    int k = (int)((b - a) / PHI);
    return a == (int)(k * PHI) && b == (int)(k * PHI * PHI);
}
}  // namespace WythoffGame

// 5. Nim游戏变种：每次最多取某一堆的一半
namespace HalfNim {
// SG值需要通过记忆化搜索计算
map<vector<int>, int> sg_memo;

int calc_sg(vector<int> piles) {
    sort(piles.begin(), piles.end());
    piles.erase(remove(piles.begin(), piles.end(), 0), piles.end());

    if (piles.empty()) return 0;
    if (sg_memo.count(piles)) return sg_memo[piles];

    set<int> reachable;
    for (int i = 0; i < (int)piles.size(); i++) {
        for (int take = 1; take <= piles[i] / 2; take++) {
            vector<int> next = piles;
            next[i] -= take;
            reachable.insert(calc_sg(next));
        }
    }

    int mex = 0;
    while (reachable.count(mex)) mex++;
    return sg_memo[piles] = mex;
}
}  // namespace HalfNim

// 使用示例
void solve() {
    // 基础Nim游戏
    vector<int> piles = {3, 4, 5};
    cout << "Nim game: " << (Nim::is_winning(piles) ? "First" : "Second")
         << " wins\n";

    // Bash博弈
    int n = 10, m = 3;
    cout << "Bash game: " << (BashGame::first_win(n, m) ? "First" : "Second")
         << " wins\n";

    // Wythoff博弈
    int a = 3, b = 5;
    cout << "Wythoff game: "
         << (WythoffGame::is_losing(a, b) ? "Losing" : "Winning")
         << " position\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
