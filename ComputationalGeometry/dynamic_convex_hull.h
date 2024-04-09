class DynamicConvexHull {
    struct less_x {
        bool operator()(const Point &A, const Point &B) const { return A.x == B.x ? A.y < B.y : A.x < B.x; }
    };
    struct greater_x {
        bool operator()(const Point &A, const Point &B) const { return A.x == B.x ? A.y > B.y : A.x > B.x; }
    };
    std::set<Point, less_x> lower_;
    std::set<Point, greater_x> upper_;
    template<typename Set> auto contain(const Set &s, const Point &P) const {
        if (s.size() < 2) { return PointShapeRelation::outside; }
        auto it = s.lower_bound(P);
        if (it != s.end() && *it == P) { return PointShapeRelation::on; }
        if (it == s.begin() || it == s.end()) { return PointShapeRelation::outside; }
        return PointShapeRelation(side_of_line(P, {*prev(it), *it}));
    }
    template<typename Set> auto insert(Set &s, const Point &P) {
        if (contain(s, P) != PointShapeRelation::outside) { return; }
        s.insert(P);
        auto it = s.find(P);
        if (auto i = next(it); i != s.end()) {
            auto j = next(i);
            while (j != s.end() && side_of_line(P, {*i, *j}) != Side::left) {
                s.erase(i);
                i = j;
                ++j;
            }
        }
        if (auto i = (typename Set::reverse_iterator) it; i != s.rend()) {
            auto j = next(i);
            while (j != s.rend() && side_of_line(P, {*j, *i}) != Side::left) { i = (decltype(i)) s.erase((j++).base()); }
        }
    }
  public:
    auto contain(const Point &P) const { return std::max(contain(lower_, P), contain(upper_, P)); }
    auto insert(const Point &P) {
        insert(lower_, P);
        insert(upper_, P);
    }
};
