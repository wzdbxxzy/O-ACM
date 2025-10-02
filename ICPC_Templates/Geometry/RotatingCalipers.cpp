// Rotating Calipers (旋转卡壳)
// 功能：凸包上的最远点对、最小矩形覆盖等
// 复杂度：O(n)（在凸包上）
// 应用：凸包直径、最大三角形、最小矩形

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(double t) const { return Point(x * t, y * t); }
    Point operator/(double t) const { return Point(x / t, y / t); }

    double dot(const Point& p) const { return x * p.x + y * p.y; }
    double cross(const Point& p) const { return x * p.y - y * p.x; }
    double norm() const { return sqrt(x * x + y * y); }
    double norm2() const { return x * x + y * y; }

    Point rotate(double angle) const {
        double c = cos(angle), s = sin(angle);
        return Point(x * c - y * s, x * s + y * c);
    }
};

// Andrew算法求凸包
vector<Point> convex_hull(vector<Point> points) {
    int n = points.size();
    if (n <= 2) return points;

    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    vector<Point> hull;

    // 下凸壳
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 &&
               (hull.back() - hull[hull.size() - 2])
                       .cross(points[i] - hull.back()) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // 上凸壳
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while (hull.size() > lower_size &&
               (hull.back() - hull[hull.size() - 2])
                       .cross(points[i] - hull.back()) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    hull.pop_back();  // 移除重复的起点
    return hull;
}

// 旋转卡壳：求凸包直径（最远点对）
double convex_diameter(const vector<Point>& hull) {
    int n = hull.size();
    if (n <= 1) return 0;
    if (n == 2) return (hull[1] - hull[0]).norm();

    double ans = 0;
    int j = 1;

    for (int i = 0; i < n; i++) {
        // 对于边 hull[i] -> hull[i+1]
        Point edge = hull[(i + 1) % n] - hull[i];

        // 旋转 j 直到距离不再增加
        while (true) {
            Point cur = hull[j] - hull[i];
            Point nxt = hull[(j + 1) % n] - hull[i];

            // 如果下一个点到边的距离更大，继续旋转
            if (edge.cross(nxt) > edge.cross(cur)) {
                j = (j + 1) % n;
            } else {
                break;
            }
        }

        ans = max(ans, (hull[j] - hull[i]).norm());
        ans = max(ans, (hull[j] - hull[(i + 1) % n]).norm());
    }

    return ans;
}

// 旋转卡壳：最大三角形面积
double max_triangle_area(const vector<Point>& hull) {
    int n = hull.size();
    if (n < 3) return 0;

    double ans = 0;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (i + 2) % n;

        for (; j < n; j = (j + 1) % n) {
            // 固定 hull[i] 和 hull[j]
            // 旋转 k 找到最大面积
            while (true) {
                double area1 =
                    abs((hull[j] - hull[i]).cross(hull[k] - hull[i]));
                double area2 =
                    abs((hull[j] - hull[i]).cross(hull[(k + 1) % n] - hull[i]));

                if (area2 > area1) {
                    k = (k + 1) % n;
                } else {
                    break;
                }
            }

            double area =
                abs((hull[j] - hull[i]).cross(hull[k] - hull[i])) / 2.0;
            ans = max(ans, area);

            if (j == i + n - 1) break;
        }
    }

    return ans;
}

// 旋转卡壳：最小矩形覆盖
struct Rectangle {
    Point corners[4];
    double area;

    void calc_area() {
        area =
            (corners[1] - corners[0]).norm() * (corners[2] - corners[1]).norm();
    }
};

Rectangle minimum_bounding_rectangle(const vector<Point>& hull) {
    int n = hull.size();
    Rectangle ans;
    ans.area = 1e18;

    if (n <= 2) {
        // 特殊处理
        return ans;
    }

    int j = 1, k = 1, l = 1;

    for (int i = 0; i < n; i++) {
        Point edge = hull[(i + 1) % n] - hull[i];
        edge = edge / edge.norm();
        Point perp(-edge.y, edge.x);

        // j: 最右边的点
        while ((hull[(j + 1) % n] - hull[i]).dot(edge) >
               (hull[j] - hull[i]).dot(edge)) {
            j = (j + 1) % n;
        }

        // k: 最上面的点
        while ((hull[(k + 1) % n] - hull[i]).dot(perp) >
               (hull[k] - hull[i]).dot(perp)) {
            k = (k + 1) % n;
        }

        if (i == 0) l = k;

        // l: 最左边的点
        while ((hull[(l + 1) % n] - hull[i]).dot(edge) <
               (hull[l] - hull[i]).dot(edge)) {
            l = (l + 1) % n;
        }

        // 计算矩形
        double width = (hull[j] - hull[l]).dot(edge);
        double height = (hull[k] - hull[i]).dot(perp);
        double area = width * height;

        if (area < ans.area) {
            ans.area = area;

            // 计算矩形四个顶点
            Point base = hull[i];
            ans.corners[0] = base + edge * (hull[l] - base).dot(edge);
            ans.corners[1] = base + edge * (hull[j] - base).dot(edge);
            ans.corners[2] = ans.corners[1] + perp * height;
            ans.corners[3] = ans.corners[0] + perp * height;
        }
    }

    return ans;
}

// 旋转卡壳：凸包的宽度（最近对边距离）
double convex_width(const vector<Point>& hull) {
    int n = hull.size();
    if (n <= 2) return 0;

    double ans = 1e18;
    int j = 1;

    for (int i = 0; i < n; i++) {
        Point edge = hull[(i + 1) % n] - hull[i];

        while (abs(edge.cross(hull[(j + 1) % n] - hull[i])) >
               abs(edge.cross(hull[j] - hull[i]))) {
            j = (j + 1) % n;
        }

        double dist = abs(edge.cross(hull[j] - hull[i])) / edge.norm();
        ans = min(ans, dist);
    }

    return ans;
}

// 使用示例
void solve() {
    int n;
    cin >> n;

    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }

    vector<Point> hull = convex_hull(points);

    cout << fixed << setprecision(10);

    // 凸包直径
    cout << "Diameter: " << convex_diameter(hull) << "\n";

    // 最大三角形面积
    cout << "Max triangle area: " << max_triangle_area(hull) << "\n";

    // 最小矩形覆盖
    Rectangle rect = minimum_bounding_rectangle(hull);
    cout << "Min rectangle area: " << rect.area << "\n";

    // 凸包宽度
    cout << "Width: " << convex_width(hull) << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
