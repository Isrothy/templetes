struct Couple {
    double Re, Im;
    Couple(double Re_ = 0, double Im_ = 0) : Re(Re_), Im(Im_) {}
    Couple operator+(Couple const &_) const { return (Couple){Re + _.Re, Im + _.Im}; }
    Couple operator-(Couple const &_) const { return (Couple){Re - _.Re, Im - _.Im}; }
    Couple operator*(Couple const &_) const { return (Couple){Re * _.Re - Im * _.Im, Re * _.Im + Im * _.Re}; }
};
auto dft(std::vector<Couple> a) {
    constexpr auto pi = std::numbers::pi;
    auto n = (int) a.size();
    for (int i = 0, j = 0; i < n; ++i) {
        if (i < j) { std::swap(a[i], a[j]); }
        for (int k = n >> 1; (j ^= k) < k; k >>= 1) ;
    }
    std::vector<Couple> w(n);
    w[0] = 1;
    for (int i = 1; i < n; i <<= 1) {
        Couple wn{cos(pi / i), sin(pi / i)};
        for (int j = i - 2; j >= 0; j -= 2) {
            w[j] = w[j >> 1];
            w[j + 1] = wn * w[j];
        }
        for (int j = 0; j < n; j += i << 1) {
            auto *p = a.data() + j, *q = a.data() + j + i;
            for (int k = 0; k < i; ++k) {
                auto x = q[k] * w[k];
                q[k] = p[k] - x;
                p[k] = p[k] + x;
            }
        }
    }
    return a;
}
auto mtt(const std::vector<int> &A, const std::vector<int> &B, int mod) {
    auto n = A.size(), m = B.size();
    auto l = std::bit_ceil(n + m - 1);
    std::vector<Couple> a(l), b(l), c(l), d(l);
    for (int i = 0; i < n; ++i) { a[i] = {(double) (A[i] & 32767), (double) (A[i] >> 15)}; }
    for (int i = 0; i < m; ++i) { b[i] = {(double) (B[i] & 32767), (double) (B[i] >> 15)}; }
    a = dft(a); b = dft(b);
    for (int i = 0; i < l; ++i) {
        auto j = (l - 1) & (l - i);
        c[j] = Couple{0.5 * (a[i].Re + a[j].Re), 0.5 * (a[i].Im - a[j].Im)} * b[i];
        d[j] = (Couple){0.5 * (a[j].Im + a[i].Im), 0.5 * (a[j].Re - a[i].Re)} * b[i];
    }
    c = dft(c); d = dft(d);
    std::vector<int> C(n + m - 1);
    for (int i = 0; i < n + m - 1; ++i) {
        auto u = std::llrint(c[i].Re / l), v = std::llrint(c[i].Im / l);
        auto x = std::llrint(d[i].Re / l), y = std::llrint(d[i].Im / l);
        C[i] = (u + ((v + x) << 15) + (y % mod << 30)) % mod;
    }
    return C;
}
