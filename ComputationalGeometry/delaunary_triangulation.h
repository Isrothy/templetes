#include "2D_computational_geometry.h"
#include <array>
#include <list>
class DelaunayGraph {
    class QuadEdge;
    using EdgeIt = std::list<QuadEdge>::iterator;
    class QuadEdge {
        Point *orig_;
        EdgeIt rot_{}, onext_{};
    public:
        explicit QuadEdge(Point *origin) : orig_(origin) {}
#define DEFINE_ACCESSOR(name, expr)    \
    auto name() const { return expr; } \
    auto &name() { return expr; }
        DEFINE_ACCESSOR(rot, rot_);
        DEFINE_ACCESSOR(onext, onext_);
        DEFINE_ACCESSOR(rev, rot()->rot());
        DEFINE_ACCESSOR(oprev, rot()->onext()->rot());
        DEFINE_ACCESSOR(lnext, rot()->rev()->onext()->rot());
        DEFINE_ACCESSOR(rprev, rev()->onext());
        DEFINE_ACCESSOR(orig, orig_)
        DEFINE_ACCESSOR(dest, rev()->orig());
#undef DEFINE_ACCESSOR
        auto segment() const { return Segment{*orig(), *dest()}; }
    };
    std::list<QuadEdge> edges_;
    auto new_edge(Point *origin) {
        edges_.emplace_front(origin);
        return edges_.begin();
    }
    auto add_edge(Point *from, Point *to) {
        auto e0 = new_edge(from);
        auto e1 = new_edge(nullptr);
        auto e2 = new_edge(to);
        auto e3 = new_edge(nullptr);
        std::tie(e0->rot(), e1->rot(), e2->rot(), e3->rot()) = {e1, e2, e3, e0};
        std::tie(e0->onext(), e1->onext(), e2->onext(), e3->onext()) = {e0, e3, e2, e1};
        return e0;
    }
    static auto splice(EdgeIt a, EdgeIt b) {
        std::swap(a->onext()->rot()->onext(), b->onext()->rot()->onext());
        std::swap(a->onext(), b->onext());
    }
    auto delete_edge(EdgeIt e) {
        splice(e, e->oprev());
        splice(e->rev(), e->rev()->oprev());
        edges_.erase(e->rev()->rot());
        edges_.erase(e->rev());
        edges_.erase(e->rot());
        edges_.erase(e);
    }
    auto connect(EdgeIt a, EdgeIt b) {
        auto e = add_edge(a->dest(), b->orig());
        splice(e, a->lnext());
        splice(e->rev(), b);
        return e;
    }
    static auto determinate(const std::array<std::array<double, 3>, 3> &matrix) {
        return matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] + matrix[0][2] * matrix[1][0] * matrix[2][1] - matrix[0][2] * matrix[1][1] * matrix[2][0]
               - matrix[0][1] * matrix[1][0] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1];
    }
    static auto in_circle(const Point &P, const Triangle &t) {
        auto [A, B, C] = t;
        if (sign(det(B - A, C - A)) < 0) { std::swap(B, C); }
        auto a = A - P;
        auto b = B - P;
        auto c = C - P;
        return sign(determinate({{
                   {a.x, a.y, a.len2()},
                   {b.x, b.y, b.len2()},
                   {c.x, c.y, c.len2()},
               }}))
               > 0;
    }
    auto build(std::span<Point *> points) -> std::pair<EdgeIt, EdgeIt> {
        auto n = points.size();
        if (n == 2) {
            auto e = add_edge(points[0], points[1]);
            return {e, e->rev()};
        }
        if (n == 3) {
            auto a = add_edge(points[0], points[1]);
            auto b = add_edge(points[1], points[2]);
            splice(a->rev(), b);
            switch (side_of_line(*points[1], {*points[0], *points[2]})) {
                case Side::left: {
                    auto c = connect(b, a);
                    return {c->rev(), c};
                }
                case Side::right: {
                    connect(b, a);
                    return {a, b->rev()};
                }
                case Side::on: {
                    return {a, b->rev()};
                }
            }
        }
        auto mid = n >> 1;
        auto [ldo, ldi] = build(points.subspan(0, mid));
        auto [rdi, rdo] = build(points.subspan(mid, n - mid));
        while (true) {
            if (side_of_line(*rdi->orig(), ldi->segment()) == Side::left) {
                ldi = ldi->lnext();
            } else if (side_of_line(*ldi->orig(), rdi->segment()) == Side::right) {
                rdi = rdi->rprev();
            } else {
                break;
            }
        }
        auto base = connect(rdi->rev(), ldi);
        if (ldi->orig() == ldo->orig()) { ldo = base->rev(); }
        if (rdi->orig() == rdo->orig()) { rdo = base; }
        while (true) {
            auto lcand = base->rprev();
            auto valid_l = side_of_line(*lcand->dest(), base->segment()) == Side::right;
            if (valid_l) {
                while (in_circle(*lcand->onext()->dest(), {*base->orig(), *base->dest(), *lcand->dest()})) {
                    auto t = lcand->onext();
                    delete_edge(lcand);
                    lcand = t;
                }
            }
            auto rcand = base->oprev();
            auto valid_r = side_of_line(*rcand->dest(), base->segment()) == Side::right;
            if (valid_r) {
                while (in_circle(*rcand->oprev()->dest(), {*base->orig(), *base->dest(), *rcand->dest()})) {
                    auto t = rcand->oprev();
                    delete_edge(rcand);
                    rcand = t;
                }
            }
            if (!valid_l && !valid_r) { break; }
            if (!valid_l || (valid_r && in_circle(*rcand->dest(), {*base->orig(), *base->dest(), *lcand->dest()}))) {
                base = connect(rcand, base->rev());
            } else {
                base = connect(base->rev(), lcand->rev());
            }
        }
        return {ldo, rdo};
    }
public:
    explicit DelaunayGraph(std::span<Point> points) {
        auto n = points.size();
        if (n < 2) { return; }
        std::vector<Point *> a(n);
        std::iota(a.begin(), a.end(), points.data());
        std::sort(a.begin(), a.end(), [](const Point *P, const Point *Q) { return P->x != Q->x ? P->x < Q->x : P->y < Q->y; });
        build(a);
    }
    auto edges() const {
        std::vector<std::tuple<Point *, Point *, double>> edges;
        for (const auto &e: edges_) {
            if (e.orig() < e.dest() && e.orig() != nullptr && e.dest() != nullptr) { edges.emplace_back(e.orig(), e.dest(), len(e.segment())); }
        }
        return edges;
    }
};
