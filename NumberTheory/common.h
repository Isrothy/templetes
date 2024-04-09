int32_t constexpr mul_mod(int32_t a, int32_t b, int32_t mod) { return static_cast<int32_t>(static_cast<int64_t>(a) * b % mod); }
int64_t constexpr mul_mod(int64_t a, int64_t b, int64_t mod) {
#ifdef __SIZEOF_INT128__
    return static_cast<int64_t>(static_cast<__int128>(a) * b % mod);
#else
    int64_t res = 0;
    for (b = (b % mod + mod) % mod; b; b >>= 1) {
        if (b & 1) { res = (res + a) % mod; }
        a = (a + a) % mod;
    }
    return res;
#endif
}
template<typename T> constexpr std::tuple<T, T, T> ex_gcd(const T &a, const T &b) {
    if (b == 0) { return {a, T(1), T(0)}; }
    auto [d, x, y] = ex_gcd(b, a % b);
    return {d, y, x - a / b * y};
}
template<typename T, typename U> constexpr T power(T x, U k, const std::function<T(T, T)> &multiply) {
    T res{1};
    for (; k; k >>= 1) {
        if (k & 1) { res = multiply(res, x); }
        x = multiply(x, x);
    }
    return res;
}
template<typename T, typename U> constexpr T power(T x, U k, const T &mod) {
    T res{1};
    for (; k; k >>= 1) {
        if (k & 1) { res = mul_mod(res, x, mod); }
        x = mul_mod(x, x, mod);
    }
    return res;
}
template<typename T> constexpr T inverse(const T &a, const T &mod) {
    auto [d, x, y] = ex_gcd(a, mod);
    return d * x;
}
template<typename T> std::optional<T> bsgs(const T &a, T b, const T &mod) {
    if (mod == 1) { return 0; }
    T w{1}, x{1}, s{static_cast<T>(std::sqrt(mod)) + 1};
    std::unordered_map<T, T> map;
    map.reserve(s);
    for (T k = 1; k <= s; ++k) {
        b = mul_mod(b, a, mod);
        w = mul_mod(w, a, mod);
        map[b] = k;
    }
    for (T k = 1; k <= s; ++k) {
        x = mul_mod(x, w, mod);
        if (map.contains(x)) { return (k * s - map[x]) % (mod - 1); }
    }
    return std::nullopt;
}
template<typename T> std::optional<T> ex_bsgs(T a, T b, const T &mod) {
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    if (b == 1 || mod == 1) { return 0; }
    auto d = gcd(a, mod);
    if (b % d) { return std::nullopt; }
    if (d == 1) { return bsgs(a, b, mod); }
    auto g = inverse(a / d, mod / d);
    auto x = ex_bsgs(a, b / d * g, mod / d);
    if (!x.has_value()) { return std::nullopt; }
    return x.value() + 1;
}
template<typename T> struct Crt {
    std::vector<T> mt;
    T m{};
    Crt() = default;
    explicit Crt(std::span<T> a) : mt(a.size()) {
        m = std::accumulate(a.begin(), a.end(), T{1}, std::multiplies<>());
        for (size_t i = 0; i < a.size(); ++i) {
            auto mi = m / a[i];
            mt[i] = mi * inverse(mi, a[i]) % m;
        }
    }
    T query(std::span<T> b) {
        T res = 0;
        for (size_t i = 0; i < mt.size(); ++i) { res = (res + b[i] * mt[i]) % m; }
        return res;
    }
};
template<typename T> auto ex_crt(T a1, T m1, T a2, T m2) -> std::optional<std::pair<T, T>> {
    auto [d, x, y] = ex_gcd(m1, m2);
    if ((a2 - a1) % d) { return std::nullopt; }
    auto m = m1 / d * m2;
    auto t = ((a2 - a1) / d * x % (m2 / d)) * m1 % m;
    auto a = (a1 + t) % m;
    if (a < 0) { a += m; }
    return std::pair{a, m};
}
auto sieve_of_euler(size_t n) {
    std::vector<int> primes;
    std::vector<bool> is_composite(n);
    primes.reserve(static_cast<size_t>(static_cast<double>(n) / std::log(n)));
    primes.push_back(0);
    for (int i = 2; i < n; ++i) {
        if (!is_composite[i]) { primes.push_back(i); }
        for (size_t j = 1; j < primes.size() && i * primes[j] < n; ++j) {
            is_composite[i * primes[j]] = true;
            if (i % primes[j] == 0) { break; }
        }
    }
    return std::pair{primes, is_composite};
}
template<typename T> bool miller_rabin_test(const T &n) {
    if (n < 2) { return false; }
    if (~n & 1) { return n == 2; }
    T d = n - 1, s = 0;
    for (; ~d & 1; d >>= 1) { ++s; }
    static constexpr auto p = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    return std::none_of(p.begin(), p.end(), [=](auto a) {
        if (a == n) { return false; }
        T x = power<T, T>(a, d, n);
        if (x == 1 || x == n - 1) { return false; }
        for (int i = 1; i < s; ++i) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { return false; }
        }
        return true;
    });
}
template<typename T> T pollard_rho(const T &n) {
    if (miller_rabin_test(n)) { return n; }
    for (auto p: {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}) {
        if (n % p == 0) { return p; }
    }
    std::mt19937 mt_rand(std::random_device{}());
    std::uniform_int_distribution<T> dist(1, n - 1);
    while (true) {
        auto c = dist(mt_rand);
        auto f = [&](const T &x) { return (mul_mod(x, x, n) + c) % n; };
        auto t = f(0), r = f(t);
        int steps = 1;
        while (t != r) {
            T prod{1};
            for (int i = 0; i < steps; ++i) {
                if (auto tmp = mul_mod(prod, std::abs(t - r), n)) {
                    prod = tmp;
                    t = f(t);
                    r = f(f(r));
                } else {
                    break;
                }
            }
            if (auto d = std::gcd(prod, n); d != 1) { return d; }
            steps = std::min(128, steps << 1);
        }
    }
}
template<typename T> auto prime_factors(const T &n) {
    std::queue<T> q;
    std::vector<T> res;
    for (q.push(n); !q.empty(); q.pop()) {
        if (auto x = q.front(); miller_rabin_test(x)) {
            res.push_back(x);
        } else {
            auto d = pollard_rho(x);
            q.push(d);
            q.push(x / d);
        }
    }
    std::sort(res.begin(), res.end());
    res.erase(std::unique(res.begin(), res.end()), res.end());
    return res;
}
template<typename T> std::optional<T> primitive_root(T n) {
    if (n == 2 || n == 4) { return n - 1; }
    if (n == 1 || (n & 3) == 0) { return std::nullopt; }
    auto a = prime_factors(n);
    if (2 < a.size() || (a.size() == 2 && a[0] != 2)) { return std::nullopt; }
    T m = a.size() == 2 ? n / 2 / a[1] * (a[1] - 1) : n / a[0] * (a[0] - 1);
    auto b = prime_factors(m);
    for (T g{2}; g < n; ++g) {
        if (power(g, m, n) == 1 && std::all_of(b.begin(), b.end(), [&](auto p) { return power(g, m / p, n) != 1; })) { return g; }
    }
    return std::nullopt;
}
