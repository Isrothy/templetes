class Min25 {
    int64_t n, s, m;
    std::vector<int64_t> prime, h0, h1, w, h;
    size_t index(int64_t x) const { return x <= s ? x : m - n / x + 1; }
    int64_t dfs_mu(int64_t x, size_t k) const {
        if (x <= prime[k]) { return 0; }
        auto res = h[index(x)] - h[prime[k]];
        for (size_t i = k + 1; i < prime.size() && prime[i] * prime[i] <= x; ++i) { res = res - dfs_mu(x / prime[i], i); }
        return res;
    }
    int64_t dfs_phi(int64_t x, size_t k) const {
        if (x <= prime[k]) { return 0; }
        int64_t res = h[index(x)] - h[prime[k]];
        for (size_t i = k + 1; i < prime.size() && prime[i] * prime[i] <= x; ++i) {
            for (int64_t p = prime[i], d = prime[i], g = p - 1; d <= x / p; d *= p) {
                res = res + g * (dfs_phi(x / d, i)) + g * p;
                g = g * p;
            }
        }
        return res;
    }
public:
    explicit Min25(int64_t n) : n(n), s((int) std::sqrt(n)), m(0), h0(2 * s + 2), h1(2 * s + 2), w(2 * s + 2), h(2 * s + 2) {
        std::vector<bool> is_composite(s + 1);
        prime.push_back(0);
        for (int i = 2; i <= s; ++i) {
            if (!is_composite[i]) {
                prime.push_back(i);
                for (auto j = 2 * i; j <= s; j += i) { is_composite[j] = true; }
            }
        }
        for (int64_t x = 1; x <= n; x = w[m] + 1) {
            ++m;
            auto tmp = w[m] = n / (n / x);
            h0[m] = tmp;
            h1[m] = (tmp * (tmp + 1)) >> 1;
        }
        for (size_t i = 1; i < prime.size(); ++i) {
            for (auto j = m, p = prime[i]; w[j] >= p * p; --j) {
                auto k = index(w[j] / p);
                h0[j] -= h0[k] - h0[p - 1];
                h1[j] -= (h1[k] - h1[p - 1]) * p;
            }
        }
    }
    int64_t sum_of_phi() {
        for (int i = 2; i <= m; ++i) { h[i] = h1[i] - h0[i]; }
        return 1 + dfs_phi(n, 0);
    }
    int64_t sum_of_mu() {
        for (int i = 2; i <= m; ++i) { h[i] = 1 - h0[i]; }
        return 1 + dfs_mu(n, 0);
    }
};
