template<int64_t mod> constexpr auto Euclidean_like(int64_t n, int64_t a, int64_t b, int64_t c) -> std::tuple<int64_t, int64_t, int64_t> {
    // tuple(sum_{x=0}^n (ax+b)/c,sum_{x=0}^n ((ax+b)/c)^2,sum_{x=0}^n x*((ax+b)/c))
    constexpr auto inv2 = inverse<int64_t>(2, mod);
    constexpr auto inv6 = inverse<int64_t>(6, mod);
    auto x = a / c % mod, y = b / c % mod;
    auto s0 = (n + 1) % mod;
    auto s1 = n * (n + 1) % mod * inv2 % mod;
    auto s2 = n * (n + 1) % mod * (2 * n + 1) % mod * inv6 % mod;
    auto m = (a * n + b) / c;
    if (a == 0) {
        return {y * s0 % mod, y * s1 % mod, y * y % mod * s0 % mod};
    } else if (a >= c || b >= c) {
        auto [f, g, h] = Euclidean_like<mod>(n, a % c, b % c, c);
        return {(f + x * s1 + y * s0) % mod, (g + x * s2 + y * s1) % mod, (h + 2 * y * f % mod + 2 * x * g % mod + x * x % mod * s2 % mod + 2 * x * y % mod * s1 % mod + y * y % mod * s0 % mod) % mod};
    } else {
        auto [f, g, h] = Euclidean_like<mod>(m - 1, c, c - b - 1, a);
        return {(m * n - f) % mod, (m * s1 - inv2 * h - inv2 * f) % mod, (m * (m + 1) % mod * n - 2 * g - 2 * f - m * n % mod + f) % mod};
    }
}
