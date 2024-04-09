template<typename T> class ExLucas {
    std::vector<T> p, pk, mt;
    std::vector<std::vector<T>> s;
    Crt<T> crt;
    T mod;
    T f(T n, const T &p, const T &mod, std::span<T> s) {
        T res{1};
        for (; n; n /= p) { res = res * power(s[mod], n / mod, mod) % mod * s[n % mod] % mod; }
        return res;
    }
    T g(T n, const T &p) {
        T res{0};
        for (; n; n /= p) { res += n / p; }
        return res;
    }
  public:
    explicit ExLucas(T mod) : mod(mod) {
        auto m = mod;
        auto add_prime = [&](T i) {
            p.push_back(i);
            pk.push_back(1);
            for (; m % i == 0; m /= i) { pk.back() *= i; }
            s.emplace_back(pk.back() + 1);
            auto &v = s.back();
            v[0] = 1;
            for (T j{1}; j <= pk.back(); ++j) { v[j] = j % i == 0 ? v[j - 1] : v[j - 1] * j % pk.back(); }
        };
        for (T i{2}; i * i <= m; ++i) {
            if (mod % i == 0) { add_prime(i); }
        }
        if (m != 1) { add_prime(m); }
        crt = Crt<T>(pk);
    }
    T combination(T n, T m) {
        if (m > n || m < 0) { return 0; }
        std::vector<T> b;
        for (size_t i = 0; i < s.size(); ++i) {
            auto x = f(n, p[i], pk[i], s[i]);
            auto y = inverse(f(n - m, p[i], pk[i], s[i]), pk[i]);
            auto z = inverse(f(m, p[i], pk[i], s[i]), pk[i]);
            auto r = g(n, p[i]) - g(m, p[i]) - g(n - m, p[i]);
            b.push_back(x * y % pk[i] * z % pk[i] * power(p[i], r, pk[i]) % mod);
        }
        return (crt.query(b) + mod) % mod;
    }
};
