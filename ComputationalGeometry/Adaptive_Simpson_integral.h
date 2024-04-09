auto simpson(double l, double r, const std::function<double(double)> &f) {
    auto mid = (l + r) / 2;
    return (r - l) * (f(l) + 4 * f(mid) + f(r)) / 6;
}
auto asr(double l, double r, const std::function<double(double)> &f, std::optional<double> tmp = std::nullopt) {
    auto mid = (l + r) / 2;
    auto sl = simpson(l, mid, f), sr = simpson(mid, r, f);
    if (tmp.has_value() && (sl + sr - tmp.value()) < EPS) { return sl + sr + (sl + sr - tmp.value()); }
    return asr(l, mid, f, sl) + asr(mid, r, f, sr);
}
