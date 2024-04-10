auto asr_(double l, double r, double mid, double fl, double fr, double fmid, double eps, const std::function<double(double)> &f) {
    auto simpson = [](auto l, auto r, auto fl, auto fr, auto fmid) { return (r - l) * (fl + 4 * fmid + fr) / 6; };
    auto one_quarter = (l + mid) / 2, three_quarter = (mid + r) / 2;
    auto f_one_quarter = f(one_quarter), f_three_quarter = f(three_quarter);
    auto s = simpson(l, r, fl, fr, fmid);
    auto sl = simpson(l, mid, fl, fmid, f_one_quarter), sr = simpson(mid, r, fmid, fr, f_three_quarter);
    if (auto diff = sl + sr - s; std::fabs(diff) < 15 * eps) { return sl + sr + diff / 15; }
    return asr_(l, mid, one_quarter, fl, fmid, f_one_quarter, eps / 2, f) + asr_(mid, r, three_quarter, fmid, fr, f_three_quarter, eps / 2, f);
}
auto asr(double l, double r, double eps, const std::function<double(double)> &f) { return asr_(l, r, (l + r) / 2, f(l), f(r), f((l + r) / 2), eps, f); }
