template<typename T> int Legendre(T a, T p) {
    a = (a % p + p) % p;
    if (!a) { return 0; }
    auto t = power(a, (p - 1) >> 1, p);
    return t == 1 ? 1 : -1;
}
template<typename T> std::vector<T> quadratic_residue(T a, T p) {
    switch (Legendre(a, p)) {
        case -1: return {};
        case 0: return {0};
        case 1: break;
    }
    std::mt19937_64 gen(std::random_device{}());
    std::uniform_int_distribution<T> distrib(1, p - 1);
    while (true) {
        if (T r = distrib(gen), i2 = ((r * r) % p - a) % p; Legendre(i2, p) != 1) {
            struct Complex {
                T re, im;
                Complex(T re = 0, T im = 0) : re(re), im(im) {}
            };
            auto complex_multiply = [=](const Complex &z, const Complex &w) -> Complex { return {(z.re * w.re + z.im * w.im % p * i2) % p, (z.re * w.im + z.im * w.re) % p}; };
            auto x1 = power<Complex>({r, 1}, (p + 1) >> 1, complex_multiply).re;
            auto x2 = (p - x1) % p;
            return {x1, x2};
        }
    }
}
