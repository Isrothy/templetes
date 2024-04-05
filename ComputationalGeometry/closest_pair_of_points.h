#include "2D_computational_geometry.h"
#include <span>
namespace {
    auto cmp_y(const Point &A, const Point &B) { return A.y != B.y ? A.y < B.y : A.x < B.x; };
    auto closer(const Segment &s1, const Segment &s2) { return len2(s1) < len2(s2); }
    auto clostest_pair_of_points_helper(std::span<Point> a) -> Segment {
        auto n = a.size();
        if (n <= 3) {
            std::sort(a.begin(), a.end(), cmp_y);
            if (n == 2) { return std::make_pair(a[0], a[1]); }
            return std::min({std::make_pair(a[0], a[1]), std::make_pair(a[0], a[2]), std::make_pair(a[1], a[2])}, closer);
        }
        auto mid = n / 2;
        auto xmid = a[mid].x;
        auto ls = clostest_pair_of_points_helper(a.subspan(0, mid));
        auto rs = clostest_pair_of_points_helper(a.subspan(mid, n - mid));
        auto res = std::min(ls, rs, closer);
        auto d = len(res);
        std::inplace_merge(a.begin(), a.begin() + static_cast<ptrdiff_t>(mid), a.end(), cmp_y);
        std::deque<Point> b;
        for (const auto &P: a) {
            if (std::fabs(xmid - P.x) >= d) { continue; }
            for (const auto &Q: b) { res = std::min(res, std::make_pair(P, Q), closer); }
            while (!b.empty() && P.y - b.front().y >= d) { b.pop_front(); }
            b.push_back(P);
        }
        return res;
    }
}// namespace
auto closest_pair_of_points(std::vector<Point> a) {
    if (a.size() < 2) { throw std::runtime_error("closest_pair_of_points: a must have at least 2 points"); }
    std::sort(a.begin(), a.end(), [](const auto &P, const auto &Q) { return P.x != Q.x ? P.x < Q.x : P.y < Q.y; });
    return clostest_pair_of_points_helper(a);
}
