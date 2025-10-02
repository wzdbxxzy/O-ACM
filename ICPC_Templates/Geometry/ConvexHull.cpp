/**
 * 凸包 (Convex Hull)
 * Andrew算法求凸包
 * 复杂度：O(n log n)
 */
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

const ld eps = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    ld cross(const Point& p) const { return x * p.y - y * p.x; }

    bool operator<(const Point& p) const {
        return abs(x - p.x) < eps ? y < p.y : x < p.x;
    }
};

int sgn(ld x) { return abs(x) < eps ? 0 : (x < 0 ? -1 : 1); }

// Andrew算法求凸包
vector<Point> convexHull(vector<Point> pts) {
    int n = pts.size();
    if (n <= 2) return pts;

    sort(pts.begin(), pts.end());

    vector<Point> hull;
    // 下凸壳
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && sgn((hull.back() - hull[hull.size() - 2])
                                           .cross(pts[i] - hull.back())) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }

    // 上凸壳
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while (hull.size() > lower_size &&
               sgn((hull.back() - hull[hull.size() - 2])
                       .cross(pts[i] - hull.back())) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }

    hull.pop_back();  // 去掉重复的起点
    return hull;
}

// 旋转卡壳求最远点对
ld rotatingCalipers(vector<Point> hull) {
    int n = hull.size();
    if (n <= 1) return 0;
    if (n == 2) return (hull[0] - hull[1]).cross(hull[0] - hull[1]);

    ld maxDist = 0;
    for (int i = 0, j = 1; i < n; i++) {
        while (
            (hull[(i + 1) % n] - hull[i]).cross(hull[(j + 1) % n] - hull[j]) >
            0) {
            j = (j + 1) % n;
        }
        ld d1 = (hull[i] - hull[j]).cross(hull[i] - hull[j]);
        ld d2 =
            (hull[(i + 1) % n] - hull[j]).cross(hull[(i + 1) % n] - hull[j]);
        maxDist = max(maxDist, max(d1, d2));
    }
    return maxDist;
}
