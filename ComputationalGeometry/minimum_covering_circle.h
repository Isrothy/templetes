Circle minimum_covering_circle(std::vector<Point> a) {
    std::shuffle(a.begin(), a.end(), std::mt19937_64(std::random_device()()));
    Point O = a[0];
    double r = 0;
    for (int i = 1; i < a.size(); ++i) {
        using enum PointShapeRelation;
        if (point_circle_relation(a[i], {O, r}) == outside) {
            std::tie(O, r) = std::make_pair(a[i], 0);
            for (int j = 0; j < i; ++j) {
                if (point_circle_relation(a[j], {O, r}) == outside) {
                    O = middle(a[i], a[j]);
                    r = (O - a[j]).len();
                    for (int k = 0; k < j; ++k) {
                        if (point_circle_relation(a[k], {O, r}) == outside) { std::tie(O, r) = circumscribed_circle({a[i], a[j], a[k]}).value(); }
                    }
                }
            }
        }
    }
    return {O, r};
}
