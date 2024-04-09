template<typename Fun> void fwt_base(std::span<int> a, const Fun &f) {
    auto n = a.size();
    for (size_t i = 1; i < n; i <<= 1)
        for (size_t j = 0; j < n; j += i << 1) {
            int *p = a.data() + j, *q = a.data() + i + j;
            for (int k = 0; k < i; ++k) { std::tie(p[k], q[k]) = f(p[k], q[k]); }
        }
}
template<int Mod> void fwt_or(std::span<int> a) {
    fwt_base(a, [](int x, int y) { return std::make_pair(x, (x + y) % Mod); });
}
template<int Mod> void ifwt_or(std::span<int> a) {
    fwt_base(a, [](int x, int y) { return std::make_pair(x, (y - x) % Mod); });
}
template<int Mod> void fwt_and(std::span<int> a) {
    fwt_base(a, [](int x, int y) { return std::make_pair((x + y) % Mod, y); });
}
template<int Mod> void ifwt_and(std::span<int> a) {
    fwt_base(a, [](int x, int y) { return std::make_pair((x - y) % Mod, y); });
}
template<int Mod> void fwt_xor(std::span<int> a) {
    fwt_base(a, [](int x, int y) { return std::make_pair((x + y) % Mod, (x - y) % Mod); });
}
template<int Mod> void ifwt_xor(std::span<int> a) {
    fwt_base(a, [](int x, int y) {
        constexpr int64_t inv2 = (Mod + 1) / 2;
        return std::make_pair((x + y) * inv2 % Mod, (x - y) * inv2 % Mod);
    });
}
