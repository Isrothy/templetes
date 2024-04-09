constexpr double EPS = 1e-10;
constexpr int sign(double x) { return x < -EPS ? -1 : EPS < x; }
constexpr double sqr_diff(double a, double b) { return (a + b) * (a - b); }
struct Point {
    double x = 0, y = 0;
    Point() = default;
    Point(double x, double y) : x(x), y(y){};
    auto len2() const { return x * x + y * y; }
    auto len() const { return std::hypot(x, y); }
    Point operator-() const { return {-x, -y}; }
    Point operator*(double k) const { return {x * k, y * k}; }
    Point operator/(double k) const { return {x / k, y / k}; }
    Point unit() const { return *this / len(); }
    Point normal() const { return {-y, x}; }
    auto angle() const { return std::atan2(y, x); }
};
using Vector = Point;
using Line = std::pair<Point, Point>;
using Segment = Line;
using Circle = std::pair<Point, double>;
using Polygon = std::vector<Point>;
using Triangle = std::tuple<Point, Point, Point>;
Vector operator+(const Vector &a, const Vector &b) { return {a.x + b.x, a.y + b.y}; }
Vector operator-(const Vector &a, const Vector &b) { return {a.x - b.x, a.y - b.y}; }
Vector operator*(double k, const Vector &a) { return {a.x * k, a.y * k}; }
auto operator==(const Point &A, const Point &B) { return sign((A - B).len()) == 0; }
auto dot(const Vector &a, const Vector &b) { return a.x * b.x + a.y * b.y; }
auto det(const Vector &a, const Vector &b) { return a.x * b.y - a.y * b.x; }
auto middle(const Point &A, const Point &B) { return 0.5 * (A + B); }
auto vec(const Line &l) { return l.second - l.first; }
auto len(const Segment &s) { return vec(s).len(); }
auto len2(const Segment &s) { return vec(s).len2(); }
auto angle(const Vector &a, const Vector &b) {
    auto tmp = a.len() * b.len();
    return sign(sqrt(tmp)) == 0 ? 0 : acos(dot(a, b) / tmp);
}
enum class Side : int { left = -1, on = 0, right = 1 };
auto side_of_line(const Point &P, const Line &l) {
    const auto &[A, B] = l;
    return Side(sign(det(P - A, B - A)));
}
auto projection(const Point &P, const Line &l) {
    const auto &[A, B] = l;
    Vector v = B - A;
    return A + dot(v, P - A) * v / v.len2();
}
auto symmetry(const Point &P, const Line &l) { return 2 * projection(P, l) - P; }
auto point_line_distance(const Point &P, const Line &l) {
    const auto &[A, B] = l;
    Vector v1 = B - A, v2 = P - A;
    return std::fabs(det(v1, v2) / v1.len());
}
auto point_on_segment(const Point &P, const Segment &s) {
    const auto &[A, B] = s;
    return side_of_line(P, {A, B}) == Side::on && sign(dot(A - P, B - P)) <= 0;
}
auto point_segment_distance(const Point &P, const Segment &s) {
    const auto &[A, B] = s;
    auto v1 = B - A, v2 = P - A, v3 = P - B;
    if (sign(dot(v1, v2)) < 0) { return v2.len(); }
    if (sign(dot(v1, v3)) > 0) { return v3.len(); }
    return det(v1, v2) / v1.len();
}
auto parallel(const Line &l1, const Line &l2) { return sign(det(vec(l1), vec(l2))) == 0; }
enum class LineLineRelation { parallel, identical, intersecting };
auto line_intersection(const Line &l1, const Line &l2) -> std::pair<LineLineRelation, std::optional<Point>> {
    const auto &[A, B] = l1;
    const auto &[C, D] = l2;
    if (parallel(l1, l2)) {
        if (side_of_line(A, l2) == Side::on) { return {LineLineRelation::identical, std::nullopt}; }
        return {LineLineRelation::parallel, std::nullopt};
    }
    double s1 = det(D - A, C - A);
    double s2 = det(C - B, D - B);
    return {LineLineRelation::intersecting, A + (B - A) * (s1 / (s1 + s2))};
}
enum class SegmentSegmentRelation { disjoint, intersecting, touching };
auto segment_intersection(const Segment &s1, const Segment &s2) -> std::pair<SegmentSegmentRelation, std::optional<Point>> {
    const auto &[A, B] = s1;
    const auto &[C, D] = s2;
    auto [relation, p] = line_intersection(s1, s2);
    switch (relation) {
        using enum LineLineRelation;
        case parallel: return {SegmentSegmentRelation::disjoint, std::nullopt};
        case identical: {
            if (sign(dot(C - A, C - B)) <= 0 || sign(dot(D - A, D - B)) <= 0 || sign(dot(A - C, A - D)) <= 0 || sign(dot(B - C, B - D)) <= 0) {
                return {SegmentSegmentRelation::touching, std::nullopt};
            }
            return {SegmentSegmentRelation::disjoint, std::nullopt};
        }
        case intersecting: {
            auto O = p.value();
            if (sign(dot(O - A, O - A)) <= 0 && sign(dot(O - C, O - D)) <= 0) { return {SegmentSegmentRelation::intersecting, O}; }
            return {SegmentSegmentRelation::disjoint, std::nullopt};
        }
    }
}
auto triangle_area(const Triangle &t) {
    const auto &[A, B, C] = t;
    return det(B - A, C - A) * 0.5;
}
enum class PointShapeRelation : int { inside = -1, on = 0, outside = 1 };
auto point_circle_relation(const Point &P, const Circle &c) {
    const auto &[O, r] = c;
    auto d = (P - O).len();
    return PointShapeRelation(sign(r - d));
}
enum class CircleCircleRelation {
    identital,
    disjoint,
    externally_tangent,
    internally_tangent_1_to_2,
    internally_tangent_2_to_1,
    circle1_contains_circle2,
    circle2_contains_circle1,
    intersecting,
};
auto circie_circle_relation(const Circle &c1, const Circle &c2) {
    using enum CircleCircleRelation;
    const auto &[O1, r1] = c1;
    const auto &[O2, r2] = c2;
    auto d = (O2 - O1).len();
    if (sign(d) == 0 && sign(r1 - r2) == 0) { return identital; }
    switch (sign(d - r1 - r2)) {
        case 0: return externally_tangent;
        case 1: return disjoint;
        default:
            switch (sign(d - std::fabs(r1 - r2))) {
                case 0: return r1 > r2 ? internally_tangent_2_to_1 : internally_tangent_1_to_2;
                case -1: return r1 > r2 ? circle1_contains_circle2 : circle2_contains_circle1;
                default: return intersecting;
            }
    }
}
auto circle_circle_intersection(const Circle &c1, const Circle &c2) -> std::pair<CircleCircleRelation, std::variant<std::monostate, Point, std::pair<Point, Point>>> {
    const auto &[O1, r1] = c1;
    const auto &[O2, r2] = c2;
    auto relation = circie_circle_relation(c1, c2);
    auto d = (O2 - O1).len();
    auto d1 = 0.5 * (d + sqr_diff(r1, r2) / d);
    auto H = O1 + d1 * (O2 - O1) / d;
    switch (relation) {
        using enum CircleCircleRelation;
        case disjoint:
        case identital:
        case circle1_contains_circle2:
        case circle2_contains_circle1: return {relation, std::monostate()};
        case externally_tangent:
        case internally_tangent_1_to_2:
        case internally_tangent_2_to_1: return {relation, H};
        case intersecting: {
            auto v = (O2 - O1).unit().normal() * sqrt(sqr_diff(r1, d1));
            return {relation, std::make_pair(H + v, H - v)};
        }
    }
}
enum class CircleLineRelation { intersecting = -1, tangent = 0, disjoint = 1 };
auto circle_line_relation(const Circle &c, const Line &l) {
    const auto &[O, r] = c;
    auto d = point_line_distance(O, l);
    return CircleLineRelation(sign(r - d));
}
auto circle_line_intersection(const Circle &c, const Line &l) -> std::pair<CircleLineRelation, std::variant<std::monostate, Point, std::pair<Point, Point>>> {
    const auto &[O, r] = c;
    const auto &[A, B] = l;
    Point H = projection(O, l);
    auto relation = circle_line_relation(c, l);
    switch (relation) {
        using enum CircleLineRelation;
        case disjoint: return {relation, std::monostate()};
        case tangent: return {relation, H};
        case intersecting:
            double d = (H - O).len();
            auto v = (A - B).unit() * sqrt(sqr_diff(r, d));
            return {relation, std::make_pair(H + v, H - v)};
    }
}
auto circle_point_tangent(const Circle &c, const Point &P) -> std::pair<PointShapeRelation, std::variant<std::monostate, Point, std::pair<Point, Point>>> {
    const auto &[O, r] = c;
    auto relation = point_circle_relation(P, c);
    switch (relation) {
        using enum PointShapeRelation;
        case inside: return {relation, std::monostate()};
        case on: return {relation, P};
        case outside:
            auto d = (P - O).len();
            auto H = O + (P - O) * (r * r / d);
            auto v = (P - O).unit().normal() * sqrt(sqr_diff(r, d));
            return {relation, std::make_pair(H + v, H - v)};
    }
}
auto circumscribed_circle(const Triangle &t) {
    const auto &[A, B, C] = t;
    auto [relation, O] = line_intersection({middle(A, B), middle(A, B) + (A - B).normal()}, {middle(B, C), middle(B, C) + (B - C).normal()});
    return relation == LineLineRelation::intersecting ? std::optional<Circle>{Circle{O.value(), (O.value() - A).len()}} : std::nullopt;
}
auto inscribed_circle(const Triangle &t) {
    const auto &[A, B, C] = t;
    auto a = (B - C).len(), b = (C - A).len(), c = (A - B).len();
    auto I = (A * a + B * b + C * c) / (a + b + c);
    double d = point_line_distance(I, {A, B});
    return sign(d) == 0 ? std::nullopt : std::optional<Circle>{Circle{I, d}};
}
auto external_co_tangent(const Circle &c1, const Circle &c2) -> std::variant<std::monostate, Line, std::pair<Line, Line>> {
    const auto &[O1, r1] = c1;
    const auto &[O2, r2] = c2;
    if (r1 < r2) { return external_co_tangent(c2, c1); }
    const auto &[relation, p] = circle_point_tangent({O1, r1 - r2}, O2);
    switch (relation) {
        using enum PointShapeRelation;
        case inside: return std::monostate();
        case on: {
            const auto &P = std::get<1>(p);
            return Line{P, P + (O1 - P).normal()};
        }
        case outside:
            const auto &[P1, P2] = std::get<2>(p);
            auto v1 = (P1 - O1).unit() * r2;
            auto v2 = (P2 - O1).unit() * r2;
            return std::make_pair(Line{P1 + v1, P2 + v2}, Line{O1 + v1, O2 + v2});
    }
}
auto internal_co_tangent(const Circle &c1, const Circle &c2) -> std::variant<std::monostate, Line, std::pair<Line, Line>> {
    const auto &[O1, r1] = c1;
    const auto &[O2, r2] = c2;
    if (r1 < r2) { return internal_co_tangent(c2, c1); }
    const auto &[relation, p] = circle_point_tangent({O1, r1 + r2}, O2);
    switch (relation) {
        using enum PointShapeRelation;
        case inside: return std::monostate();
        case on: {
            const auto &P = std::get<1>(p);
            return Line{P, P + (O1 - P).normal()};
        }
        case outside: {
            const auto &[P1, P2] = std::get<2>(p);
            auto v1 = (P1 - O1).unit() * r2;
            auto v2 = (P2 - O1).unit() * r2;
            return {std::make_pair(Line{P1 - v1, O2 + v1}, Line{P2 - v2, O2 + v1})};
        }
    }
}
auto convex_hull(std::vector<Point> points) {
    if (points.size() <= 2) { return points; }
    int n = (int) points.size();
    std::vector<Point> stk(n + 1);
    std::sort(points.begin(), points.end(), [](const Point &A, const Point &B) { return A.x == B.x ? A.y < B.y : A.x < B.x; });
    int top = 0;
    stk[top++] = points[0];
    for (int i = 1; i < n; ++i) {
        while (2 <= top && side_of_line(points[i], {stk[top - 2], stk[top - 1]}) != Side::left) { --top; }
        stk[top++] = points[i];
    }
    int tmp = top;
    for (int i = n - 2; i >= 0; --i) {
        while (tmp < top && side_of_line(points[i], {stk[top - 2], stk[top - 1]}) != Side::left) { --top; }
        stk[top++] = points[i];
    }
    stk.erase(stk.begin() + top - 1, stk.end());
    stk.shrink_to_fit();
    return stk;
}
auto point_in_convex_polygon(const Point &P, const Polygon &p) {
    using enum PointShapeRelation;
    auto n = p.size();
    if (n < 3) { throw std::runtime_error("point_in_convex_polygon: polygon must have at least 3 points"); }
    if (P.x < p[0].x || (P.x == p[0].x && P.y < p[0].y)) { return outside; }
    if (side_of_line(P, {p[0], p[1]}) == Side::on) { return sign(dot(p[1] - P, p[0] - P)) <= 0 ? on : outside; }
    if (side_of_line(P, {p[0], p[n - 1]}) == Side::on) { return sign(dot(p[n - 1] - P, p[0] - P)) <= 0 ? on : outside; }
    auto i = std::upper_bound(p.begin() + 1, p.end(), P, [&](const Point &A, const Point &B) { return side_of_line(p[0], {A, B}) == Side::left; }) - p.begin();
    return PointShapeRelation(side_of_line(P, {p[i - 1], p[i]}));
}
auto minkowski_sum(const Polygon &a, const Polygon &b) {
    auto push_point = [](Polygon &v, const Point &P) {
        while (2 <= v.size() && side_of_line(P, {v[v.size() - 2], v.back()}) != Side::left) { v.pop_back(); }
        v.emplace_back(P);
    };
    auto n = a.size(), m = b.size();
    size_t i = 0, j = 0;
    Polygon c;
    c.reserve(n + m);
    push_point(c, a.front() + b.front());
    while (i < n && j < m) {
        auto u = a[(i + 1) % n] - a[i];
        auto v = b[(j + 1) % m] - b[j];
        if (0 < det(u, v)) {
            push_point(c, c.back() + u);
            ++i;
        } else {
            push_point(c, c.back() + v);
            ++j;
        }
    }
    for (; i < n; ++i) { push_point(c, c.back() + a[(i + 1) % n] - a[i]); }
    for (; j < m; ++j) { push_point(c, c.back() + b[(j + 1) % m] - b[j]); }
    push_point(c, c.front());
    c.pop_back();
    return c;
}
auto point_in_polygon(const Point &P, const Polygon &p) {
    using enum PointShapeRelation;
    bool result = false;
    for (size_t i = 0, n = p.size(); i < n; ++i) {
        auto A = p[i];
        auto B = p[(i + 1) % n];
        if (point_on_segment(P, {A, B})) { return on; }
        if (A.y > B.y) { std::swap(A, B); }
        if (sign(A.y - P.y) <= 0 && sign(P.y - B.y) < 0 && side_of_line(P, {A, B}) == Side::left) { result ^= 1; }
    }
    return result ? inside : outside;
}
auto polygon_area(const Polygon &p) {
    double result = 0;
    for (size_t i = 0, n = p.size(); i < n; ++i) { result += triangle_area({p[0], p[i], p[(i + 1) % n]}); }
    return result;
}
auto half_planes_intersection(std::vector<Line> lines) {
    using enum Side;
    if (lines.empty()) { throw std::runtime_error("half_planes_intersection: lines must not be empty"); }
    std::deque<Line> q;
    std::deque<Point> t;
    std::sort(lines.begin(), lines.end(), [](const auto &l1, const auto &l2) {
        int d = sign((l1.second - l1.first).angle() - (l2.second - l2.first).angle());
        return d == 0 ? side_of_line(l1.first, l2) == left : d < 0;
    });
    q.emplace_back(lines[0]);
    for (const auto &line: lines) {
        if (parallel(q.back(), line)) { continue; }
        while (!t.empty() && side_of_line(t.back(), line) != left) {
            t.pop_back();
            q.pop_back();
        }
        while (!t.empty() && side_of_line(t.front(), line) != left) {
            t.pop_front();
            q.pop_front();
        }
        q.emplace_back(line);
        t.emplace_back(line_intersection(q.back(), line).second.value());
    }
    while (!t.empty() && side_of_line(t.back(), q.front()) != left) {
        t.pop_back();
        q.pop_back();
    }
    if (q.size() > 1) { t.emplace_front(line_intersection(q.front(), q.back()).second.value()); }
    return std::pair{std::vector{q.begin(), q.end()}, Polygon{t.begin(), t.end()}};
}
auto polygons_union_area(const std::span<Polygon> &polygons) {
    auto n = polygons.size();
    std::vector<Line> lines;
    for (size_t i = 0; i < n; ++i) {
        for (size_t m = polygons[i].size(), j = 0; j < m; ++j) { lines.emplace_back(polygons[i][j], polygons[i][(j + 1) % m]); }
    }
    auto m = lines.size();
    std::vector<size_t> fa(m);
    std::vector<std::vector<std::tuple<size_t, Point, Side, Side>>> events(m);
    std::iota(fa.begin(), fa.end(), 0);
    std::function<size_t(size_t)> find;
    find = [&](size_t x) { return fa[x] == x ? x : fa[x] = find(fa[x]); };
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = i + 1; j < m; ++j) {
            if (auto u = find(i), v = find(j); u != v) {
                if (auto [relation, _] = line_intersection(lines[i], lines[j]); relation == LineLineRelation::identical) { fa[u] = v; }
            }
        }
    }
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = i + 1; j < m; ++j) {
            if (auto u = find(i), v = find(j); u != v) {
                if (auto [relation, I] = line_intersection(lines[u], lines[v]); relation == LineLineRelation::intersecting) {
                    const auto &[A, B] = lines[i];
                    const auto &[C, D] = lines[j];
                    auto sideA = side_of_line(A, {C, D});
                    auto sideB = side_of_line(B, {C, D});
                    auto sideC = side_of_line(C, {A, B});
                    auto sideD = side_of_line(D, {A, B});
                    if (sideA != sideB) { events[find(j)].emplace_back(i, I.value(), sideA, sideB); }
                    if (sideC != sideD) { events[find(i)].emplace_back(j, I.value(), sideC, sideD); }
                }
            }
        }
    }
    double res = 0;
    for (size_t i = 0; i < m; ++i) {
        if (find(i) != i) { continue; }
        const auto &[a, b] = lines[i];
        std::sort(events[i].begin(), events[i].end(), [&](const auto &a, const auto &b) { return std::get<0>(a) < std::get<0>(b); });
        events[i].erase(std::unique(events[i].begin(), events[i].end(), [&](const auto &a, const auto &b) { return std::get<0>(a) == std::get<0>(b); }), events[i].end());
        events[i].erase(std::remove_if(events[i].begin(), events[i].end(), [&](const auto &a) { return find(std::get<0>(a)) == i; }), events[i].end());
        std::sort(events[i].begin(), events[i].end(), [a, b](const auto &p, const auto &q) { return dot(std::get<1>(p) - a, b - a) < dot(std::get<1>(q) - a, b - a); });
        int cntl = 0, cntr = 0;
        double last;
        auto unit = (b - a).unit();
        for (auto const &[id, o, sideC, sideD]: events[i]) {
            auto dis = dot(o - a, unit);
            if (cntl != 0 && cntr == 0) { res += det(a + unit * last, a + unit * dis); }
            if (sideC == Side::left) { ++cntl; }
            if (sideD == Side::left) { --cntl; }
            if (sideC == Side::right) { --cntr; }
            if (sideD == Side::right) { ++cntr; }
            last = dis;
        }
    }
    return res / 2;
}
