/**
 * 计算几何基础 - 点和向量
 */
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

const ld eps = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(ld t) const { return Point(x * t, y * t); }
    Point operator/(ld t) const { return Point(x / t, y / t); }

    ld dot(const Point& p) const { return x * p.x + y * p.y; }
    ld cross(const Point& p) const { return x * p.y - y * p.x; }

    ld norm() const { return sqrt(x * x + y * y); }
    ld norm2() const { return x * x + y * y; }

    Point rot90() const { return Point(-y, x); }
    Point unit() const { return *this / norm(); }

    ld angle() const { return atan2(y, x); }
    Point rotate(ld rad) const {
        return Point(x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad));
    }

    bool operator<(const Point& p) const {
        return abs(x - p.x) < eps ? y < p.y : x < p.x;
    }
    bool operator==(const Point& p) const {
        return abs(x - p.x) < eps && abs(y - p.y) < eps;
    }
};

using Vector = Point;

// 叉积判断方向
int sgn(ld x) { return abs(x) < eps ? 0 : (x < 0 ? -1 : 1); }

// 点p在线段ab上（包括端点）
bool onSegment(Point p, Point a, Point b) {
    return sgn((a - p).cross(b - p)) == 0 && sgn((a - p).dot(b - p)) <= 0;
}

// 线段相交（不包括端点重合）
bool segmentIntersect(Point a, Point b, Point c, Point d) {
    ld c1 = (b - a).cross(c - a), c2 = (b - a).cross(d - a);
    ld c3 = (d - c).cross(a - c), c4 = (d - c).cross(b - c);
    return sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0;
}

// 点到直线距离
ld pointToLine(Point p, Point a, Point b) {
    return abs((b - a).cross(p - a)) / (b - a).norm();
}

// 点到线段距离
ld pointToSegment(Point p, Point a, Point b) {
    if (sgn((b - a).dot(p - a)) < 0) return (p - a).norm();
    if (sgn((a - b).dot(p - b)) < 0) return (p - b).norm();
    return pointToLine(p, a, b);
}

// 点p在多边形内（射线法）
bool inPolygon(Point p, vector<Point> poly) {
    int n = poly.size();
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if (onSegment(p, a, b)) return true;
        int k = sgn((b - a).cross(p - a));
        int u = sgn(a.y - p.y);
        int v = sgn(b.y - p.y);
        if (k > 0 && u < 0 && v >= 0) cnt++;
        if (k < 0 && v < 0 && u >= 0) cnt--;
    }
    return cnt != 0;
}

// 多边形面积
ld polygonArea(vector<Point> poly) {
    ld area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        area += poly[i].cross(poly[(i + 1) % n]);
    }
    return abs(area) / 2;
}
