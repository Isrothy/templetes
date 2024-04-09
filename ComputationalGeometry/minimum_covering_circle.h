Circle minimum_covering_circle(std::vector<Point> a) {
    std::shuffle(a.begin(), a.end(), std::mt19937_64(std::random_device()()));
    auto O = a[0];
    auto r = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (sign(sqr_diff(r, (O - a[i]).len())) < 0) {
            O = a[i];
            r = 0;
            for (size_t j = 0; j < i; ++j) {
                if (sign(sqr_diff(r, (O - a[j]).len())) < 0) {
                    O = middle(a[i], a[j]);
                    r = (O - a[j]).len();
                    for (size_t k = 0; k < j; ++k) {
                        if (sign(sqr_diff(r, (O - a[k]).len())) < 0) { std::tie(O, r) = circumscribed_circle({a[i], a[j], a[k]}).value(); }
                    }
                }
            }
        }
    }
    return {O, r};
}
