// Adaptive Simpson Integration (自适应Simpson积分)
// 功能：数值积分，自适应精度控制
// 复杂度：O(f(n) * log(1/eps))，f(n)为函数求值复杂度
// 应用：求不规则图形面积、物理问题

#include <bits/stdc++.h>
using namespace std;

const long double EPS = 1e-8;

// 函数类型定义
using Function = function<long double(long double)>;

// Simpson公式：近似计算积分
long double simpson(Function f, long double l, long double r) {
    long double mid = (l + r) / 2.0;
    return (r - l) * (f(l) + 4.0 * f(mid) + f(r)) / 6.0;
}

// 自适应Simpson积分（递归版本）
long double adaptive_simpson(Function f, long double l, long double r,
                             long double eps, long double ans) {
    long double mid = (l + r) / 2.0;
    long double left_ans = simpson(f, l, mid);
    long double right_ans = simpson(f, mid, r);
    long double total = left_ans + right_ans;

    // 如果精度足够，直接返回
    if (abs(total - ans) <= 15 * eps) {
        return total + (total - ans) / 15.0;
    }

    // 否则递归处理左右两半
    return adaptive_simpson(f, l, mid, eps / 2, left_ans) +
           adaptive_simpson(f, mid, r, eps / 2, right_ans);
}

// 外部接口
long double integrate(Function f, long double l, long double r,
                      long double eps = EPS) {
    return adaptive_simpson(f, l, r, eps, simpson(f, l, r));
}

// ========== 常见应用 ==========

// 1. 基本函数积分
namespace Example1 {
void solve() {
    // 计算 ∫[0,1] x^2 dx = 1/3
    auto f = [](long double x) { return x * x; };
    cout << fixed << setprecision(10);
    cout << integrate(f, 0, 1) << "\n";  // 输出约0.3333333333
}
}  // namespace Example1

// 2. 分式函数积分
namespace Example2 {
void solve() {
    // 计算 ∫[L,R] (cx+d)/(ax+b) dx
    long double a, b, c, d, L, R;
    cin >> a >> b >> c >> d >> L >> R;

    auto f = [=](long double x) { return (c * x + d) / (a * x + b); };

    cout << fixed << setprecision(6);
    cout << integrate(f, L, R) << "\n";
}
}  // namespace Example2

// 3. 圆的面积（极坐标积分）
namespace Example3 {
void solve() {
    long double r;
    cin >> r;

    // 计算圆的面积：∫∫ r dr dθ
    auto f = [=](long double theta) {
        return 0.5 * r * r;  // 极坐标面积元素
    };

    long double area = integrate(f, 0, 2 * acos(-1.0));
    cout << fixed << setprecision(6);
    cout << area << "\n";
}
}  // namespace Example3

// 4. 不规则图形面积（由函数围成）
namespace Example4 {
void solve() {
    // 计算由y=sin(x)和x轴在[0,π]围成的面积
    auto f = [](long double x) { return sin(x); };

    long double area = integrate(f, 0, acos(-1.0));
    cout << fixed << setprecision(6);
    cout << area << "\n";  // 输出约2
}
}  // namespace Example4

// 5. 旋转体体积
namespace Example5 {
void solve() {
    // 计算函数y=f(x)绕x轴旋转形成的体积
    // V = π∫[a,b] f(x)^2 dx

    auto f = [](long double x) { return x; };  // y = x

    auto volume_func = [&](long double x) {
        long double y = f(x);
        return acos(-1.0) * y * y;
    };

    long double volume = integrate(volume_func, 0, 1);
    cout << fixed << setprecision(6);
    cout << volume << "\n";  // 圆锥体积 π/3
}
}  // namespace Example5

// 6. 弧长计算
namespace Example6 {
void solve() {
    // 计算曲线y=f(x)在[a,b]的弧长
    // L = ∫[a,b] √(1 + f'(x)^2) dx

    auto f = [](long double x) { return x * x; };   // y = x^2
    auto df = [](long double x) { return 2 * x; };  // y' = 2x

    auto arc_func = [&](long double x) {
        long double derivative = df(x);
        return sqrt(1 + derivative * derivative);
    };

    long double arc_length = integrate(arc_func, 0, 1);
    cout << fixed << setprecision(6);
    cout << arc_length << "\n";
}
}  // namespace Example6

// 7. 物理应用：质心计算
namespace Example7 {
void solve() {
    // 计算平面图形的质心
    auto f = [](long double x) { return sin(x); };

    long double a = 0, b = acos(-1.0);

    // 面积
    long double area = integrate(f, a, b);

    // x方向质心：∫ x*f(x) dx / 面积
    auto xf = [&](long double x) { return x * f(x); };
    long double x_center = integrate(xf, a, b) / area;

    // y方向质心：∫ f(x)^2/2 dx / 面积
    auto f2 = [&](long double x) {
        long double y = f(x);
        return y * y / 2.0;
    };
    long double y_center = integrate(f2, a, b) / area;

    cout << fixed << setprecision(6);
    cout << "Center of mass: (" << x_center << ", " << y_center << ")\n";
}
}  // namespace Example7

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example1::solve();
    // Example2::solve();
    // Example3::solve();
    // Example4::solve();
    // Example5::solve();
    // Example6::solve();
    Example7::solve();

    return 0;
}
