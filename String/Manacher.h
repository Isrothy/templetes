auto manacher(std::string_view s) {
    auto n = s.size();
    std::vector<int> p(2 * n + 1);
    std::vector<char> t(2 * n + 3, '#');
    for (int i = 0; i < n; ++i) { t[2 * i + 1] = s[i]; }
    for (int i = 0, l = 0, r = 0; i <= 2 * n; ++i) {
        auto k = r < i ? 0 : std::min(p[l + r - i], r - i);
        while (0 <= i - k - 1 && i + k + 1 <= 2 * n && t[i - k - 1] == t[i + k + 1]) { ++k; }
        p[i] = k;
        if (r < i + k) {
            l = i - k;
            r = i + k;
        }
    }
    return p;
}
