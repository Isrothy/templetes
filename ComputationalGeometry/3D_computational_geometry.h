const double eps = 1e-10;
constexpr int sign(double x) { return x < -eps ? -1 : (x > eps ? 1 : 0); }
struct Point {
    double x, y, z;
    Point() = default;
    Point(double x, double y, double z) : x(x), y(y), z(z){};
    Point operator*(const double &p) const { return (Point){x * p, y * p, z * p}; }
    Point operator/(const double &p) const { return (Point){x / p, y / p, z / p}; }
    double len2() const { return x * x + y * y + z * z; }
    double len() const { return std::hypot(x, y, z); }
    Point unit() const { return *this / len(); }
};
using Vector = Point;
using Plane = std::pair<Point, Vector>;
using Line = std::pair<Point, Point>;
using Triangle = std::tuple<Point, Point, Point>;
Vector operator+(const Vector &a, const Vector &b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
Vector operator-(const Vector &a, const Vector &b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
Point operator*(const double &p, const Point &A) { return A * p; }
double dot(const Vector &a, const Vector &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vector det(const Vector &a, const Vector &b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
auto point_line_distance(const Point &P, const Line &l) {
    const auto &[A, B] = l;
    Vector v1 = B - A, v2 = P - A;
    return det(v1, v2).len() / v1.len();
}
int line_plane_intersection(const Point &P1, const Point &P2, const Point P0, const Vector &n, Point &P) {
    double s = dot(n, P2 - P1);
    if (sign(s) == 0) { return 0; }
    Vector v = P2 - P1;
    double t = dot(n, P0 - P1) / s;
    P = P1 + v * t;
    return 1;
}
auto point_plane_diatance(const Point &P, const Plane &l) {
    const auto &[P0, n] = l;
    return std::fabs(dot(P - P0, n));
}
auto point_plane_projection(const Point &P, const Plane &l) {
    const auto &[P0, n] = l;
    return P - n * dot(P - P0, n);
}
using Face = std::array<int, 3>;
auto convex_hull(std::span<Point> points) {
    auto seen = [&](const Point &p, const Face &f) {
        const auto &[i, j, k] = f;
        return dot(p - points[i], det(points[j] - points[i], points[k] - points[i])) > 0;
    };
    auto n = points.size();
    auto time_stamp = std::vector<std::vector<int>>(n, std::vector<int>(n));
    std::vector<Face> faces;
    faces.push_back({0, 1, 2});
    faces.push_back({2, 1, 0});
    for (int i = 3; i < n; ++i) {
        std::vector<Face> new_faces;
        for (const auto &face: faces) {
            if (!seen(points[i], face)) {
                new_faces.push_back(face);
            } else {
                for (int k = 0; k < 3; ++k) { time_stamp[face[k]][face[(k + 1) % 3]] = i; }
            }
        }
        for (const auto &face: faces) {
            for (int k = 0; k < 3; ++k) {
                auto x = face[k], y = face[(k + 1) % 3];
                if (time_stamp[x][y] == i && time_stamp[y][x] < i) { new_faces.push_back({x, y, i}); }
            }
        }
        faces = std::move(new_faces);
    }
    return faces;
}
