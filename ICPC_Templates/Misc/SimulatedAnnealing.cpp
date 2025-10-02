// Simulated Annealing (模拟退火)
// 功能：求解最优化问题的随机化算法
// 复杂度：不确定，通常需要多次运行
// 应用：TSP、函数最优化、NP-hard问题近似解

#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

double rand_double(double l, double r) {
    return uniform_real_distribution<double>(l, r)(rng);
}

// 通用模拟退火框架
template <typename State>
class SimulatedAnnealing {
   public:
    State current, best;
    double current_energy, best_energy;
    double T;  // 温度

    // 需要实现的虚函数
    virtual double energy(const State& s) = 0;
    virtual State neighbor(const State& s, double T) = 0;
    virtual State random_state() = 0;

    void anneal(double T_init, double T_final, double alpha, int max_iter) {
        current = random_state();
        current_energy = energy(current);
        best = current;
        best_energy = current_energy;
        T = T_init;

        while (T > T_final) {
            for (int i = 0; i < max_iter; i++) {
                State new_state = neighbor(current, T);
                double new_energy = energy(new_state);
                double delta = new_energy - current_energy;

                // Metropolis准则
                if (delta < 0 || rand_double(0, 1) < exp(-delta / T)) {
                    current = new_state;
                    current_energy = new_energy;

                    if (current_energy < best_energy) {
                        best = current;
                        best_energy = current_energy;
                    }
                }
            }

            T *= alpha;  // 降温
        }
    }
};

// 应用1：TSP问题（旅行商问题）
struct TSPState {
    vector<int> path;
};

class TSP_SA : public SimulatedAnnealing<TSPState> {
    int n;
    vector<vector<double>> dist;

   public:
    TSP_SA(const vector<pair<double, double>>& cities) {
        n = cities.size();
        dist.assign(n, vector<double>(n));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                double dx = cities[i].first - cities[j].first;
                double dy = cities[i].second - cities[j].second;
                dist[i][j] = sqrt(dx * dx + dy * dy);
            }
        }
    }

    double energy(const TSPState& s) override {
        double total = 0;
        for (int i = 0; i < n; i++) {
            total += dist[s.path[i]][s.path[(i + 1) % n]];
        }
        return total;
    }

    TSPState neighbor(const TSPState& s, double T) override {
        TSPState new_state = s;

        // 随机选择邻域操作
        int op = rng() % 3;

        if (op == 0) {
            // 2-opt: 反转一段
            int i = rng() % n;
            int j = rng() % n;
            if (i > j) swap(i, j);
            reverse(new_state.path.begin() + i, new_state.path.begin() + j + 1);
        } else if (op == 1) {
            // 交换两个城市
            int i = rng() % n;
            int j = rng() % n;
            swap(new_state.path[i], new_state.path[j]);
        } else {
            // 插入操作
            int i = rng() % n;
            int j = rng() % n;
            if (i != j) {
                int city = new_state.path[i];
                new_state.path.erase(new_state.path.begin() + i);
                new_state.path.insert(new_state.path.begin() + j, city);
            }
        }

        return new_state;
    }

    TSPState random_state() override {
        TSPState s;
        s.path.resize(n);
        iota(s.path.begin(), s.path.end(), 0);
        shuffle(s.path.begin(), s.path.end(), rng);
        return s;
    }
};

// 应用2：函数最优化
// 找f(x, y)的最小值
struct Point2D {
    double x, y;
};

class FunctionOptimization : public SimulatedAnnealing<Point2D> {
    function<double(double, double)> f;
    double x_min, x_max, y_min, y_max;

   public:
    FunctionOptimization(function<double(double, double)> f_, double x_min_,
                         double x_max_, double y_min_, double y_max_)
        : f(f_), x_min(x_min_), x_max(x_max_), y_min(y_min_), y_max(y_max_) {}

    double energy(const Point2D& s) override { return f(s.x, s.y); }

    Point2D neighbor(const Point2D& s, double T) override {
        Point2D new_state;

        // 在当前点附近随机游走，步长随温度减小
        double step = T / 100;
        new_state.x = s.x + rand_double(-step, step);
        new_state.y = s.y + rand_double(-step, step);

        // 保持在边界内
        new_state.x = max(x_min, min(x_max, new_state.x));
        new_state.y = max(y_min, min(y_max, new_state.y));

        return new_state;
    }

    Point2D random_state() override {
        Point2D s;
        s.x = rand_double(x_min, x_max);
        s.y = rand_double(y_min, y_max);
        return s;
    }
};

// 应用3：最大团问题（近似解）
class MaxClique : public SimulatedAnnealing<vector<int>> {
    int n;
    vector<vector<int>> adj;

   public:
    MaxClique(const vector<vector<int>>& graph) : adj(graph) { n = adj.size(); }

    double energy(const vector<int>& s) override {
        // 能量 = -团的大小（最小化能量 = 最大化团）
        return -static_cast<double>(s.size());
    }

    vector<int> neighbor(const vector<int>& s, double T) override {
        vector<int> new_state = s;

        // 随机添加或删除一个节点
        if (rng() % 2 == 0 && !new_state.empty()) {
            // 删除
            int idx = rng() % new_state.size();
            new_state.erase(new_state.begin() + idx);
        } else {
            // 添加
            vector<int> candidates;
            for (int v = 0; v < n; v++) {
                if (find(new_state.begin(), new_state.end(), v) ==
                    new_state.end()) {
                    // 检查v是否与当前所有节点相连
                    bool ok = true;
                    for (int u : new_state) {
                        if (!adj[u][v]) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) candidates.push_back(v);
                }
            }

            if (!candidates.empty()) {
                int v = candidates[rng() % candidates.size()];
                new_state.push_back(v);
            }
        }

        return new_state;
    }

    vector<int> random_state() override {
        vector<int> s;
        if (n > 0) {
            s.push_back(rng() % n);
        }
        return s;
    }
};

// 应用4：01背包问题（近似解）
struct KnapsackState {
    vector<int> selected;  // 选中的物品
};

class Knapsack_SA : public SimulatedAnnealing<KnapsackState> {
    int n, W;
    vector<int> w, v;

   public:
    Knapsack_SA(int n_, int W_, const vector<int>& w_, const vector<int>& v_)
        : n(n_), W(W_), w(w_), v(v_) {}

    double energy(const KnapsackState& s) override {
        int total_w = 0, total_v = 0;
        for (int i : s.selected) {
            total_w += w[i];
            total_v += v[i];
        }

        if (total_w > W) {
            return 1e9;  // 不可行解
        }

        return -total_v;  // 最小化能量 = 最大化价值
    }

    KnapsackState neighbor(const KnapsackState& s, double T) override {
        KnapsackState new_state = s;

        // 随机翻转一个物品的选择状态
        int i = rng() % n;
        auto it = find(new_state.selected.begin(), new_state.selected.end(), i);

        if (it != new_state.selected.end()) {
            new_state.selected.erase(it);
        } else {
            new_state.selected.push_back(i);
        }

        return new_state;
    }

    KnapsackState random_state() override {
        KnapsackState s;
        for (int i = 0; i < n; i++) {
            if (rng() % 2 == 0) {
                s.selected.push_back(i);
            }
        }
        return s;
    }
};

// 使用示例
void solve_tsp() {
    int n;
    cin >> n;

    vector<pair<double, double>> cities(n);
    for (int i = 0; i < n; i++) {
        cin >> cities[i].first >> cities[i].second;
    }

    TSP_SA tsp(cities);

    // 多次运行取最优
    double best_result = 1e9;
    TSPState best_path;

    for (int run = 0; run < 10; run++) {
        tsp.anneal(1000, 0.01, 0.99, 100);

        if (tsp.best_energy < best_result) {
            best_result = tsp.best_energy;
            best_path = tsp.best;
        }
    }

    cout << fixed << setprecision(6);
    cout << "Best distance: " << best_result << "\n";
    cout << "Path: ";
    for (int city : best_path.path) {
        cout << city << " ";
    }
    cout << "\n";
}

void solve_function() {
    // 最小化 f(x,y) = (x-3)^2 + (y+2)^2
    auto f = [](double x, double y) {
        return (x - 3) * (x - 3) + (y + 2) * (y + 2);
    };

    FunctionOptimization opt(f, -10, 10, -10, 10);
    opt.anneal(100, 0.01, 0.95, 100);

    cout << fixed << setprecision(6);
    cout << "Minimum at (" << opt.best.x << ", " << opt.best.y << ")\n";
    cout << "f = " << opt.best_energy << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // solve_tsp();
    solve_function();

    return 0;
}
