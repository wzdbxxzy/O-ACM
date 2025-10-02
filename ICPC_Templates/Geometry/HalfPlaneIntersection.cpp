/**
 * 半平面交 (Half-Plane Intersection)
 * 功能：求半平面交的凸多边形
 * 复杂度：O(n log n)
 */
#include <bits/stdc++.h>
using namespace std;
using ld = long double;

const ld eps = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y + p.y); }
    Point operator*(ld t) const { return Point(x * t, y * t); }
    Point operator/(ld t) const { return Point(x / t, y / t); }

    ld dot(const Point& p) const { return x * p.x + y * p.y; }
    ld cross(const Point& p) const { return x * p.y - y * p.x; }
    ld norm() const { return sqrt(x * x + y * y); }
};

struct Line {
    Point a, b;
    Line(Point a = Point(), Point b = Point()) : a(a), b(b) {}
};

int sgn(ld x) { return abs(x) < eps ? 0 : (x < 0 ? -1 : 1); }

bool pointOnLineLeft(const Point& p, const Line& l) {
    return (l.b - l.a).cross(p - l.a) > 0;
}

Point lineIntersection(const Line& l1, const Line& l2) {
    return l1.a + (l1.b - l1.a) * ((l2.b - l2.a).cross(l1.a - l2.a) /
                                   (l2.b - l2.a).cross(l1.a - l1.b));
}

int sgn_angle(const Point& d) {
    return d.y > 0 || (d.y == 0 && d.x > 0) ? 1 : -1;
}

// 半平面交
vector<Point> halfPlaneIntersection(vector<Line> lines) {
    sort(lines.begin(), lines.end(), [&](Line l1, Line l2) {
        Point d1 = l1.b - l1.a;
        Point d2 = l2.b - l2.a;

        if (sgn_angle(d1) != sgn_angle(d2)) {
            return sgn_angle(d1) == 1;
        }

        return d1.cross(d2) > 0;
    });

    deque<Line> ls;
    deque<Point> ps;

    for (auto l : lines) {
        if (ls.empty()) {
            ls.push_back(l);
            continue;
        }

        while (!ps.empty() && !pointOnLineLeft(ps.back(), l)) {
            ps.pop_back();
            ls.pop_back();
        }

        while (!ps.empty() && !pointOnLineLeft(ps[0], l)) {
            ps.pop_front();
            ls.pop_front();
        }

        if ((l.b - l.a).cross(ls.back().b - ls.back().a) == 0) {
            if ((l.b - l.a).dot(ls.back().b - ls.back().a) > 0) {
                if (!pointOnLineLeft(ls.back().a, l)) {
                    assert(ls.size() == 1);
                    ls[0] = l;
                }
                continue;
            }
            return {};
        }

        ps.push_back(lineIntersection(ls.back(), l));
        ls.push_back(l);
    }

    while (!ps.empty() && !pointOnLineLeft(ps.back(), ls[0])) {
        ps.pop_back();
        ls.pop_back();
    }

    if (ls.size() <= 2) {
        return {};
    }

    ps.push_back(lineIntersection(ls[0], ls.back()));

    return vector(ps.begin(), ps.end());
}
