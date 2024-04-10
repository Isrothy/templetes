auto berlekamp_massey(std::span<int> A) {
    auto n = A.size();
    std::vector<int> tmp(n + 1), f(n + 1), g(n + 1);
    int k = 0, last_k = 0, last_delta, last = -1;
    for (int i = 0; i < n; ++i) {
        auto delta = -A[i];
        for (int j = 1; j <= k; ++j) { delta = (delta + (long long) f[j] * A[i - j]) % mod; }
        if (!delta) { continue; }
        if (last == -1) { k = i + 1;
        } else {
            auto t = delta * power(last_delta, mod - 2) % mod;
            tmp[i - last] = (tmp[i - last] + t) % mod;
            for (int j = 1; j <= last_k; ++j) { tmp[i - last + j] = (tmp[i - last + j] - t * g[j]) % mod; }
            int p = last_k;
            last_k = k;
            k = std::max(k, i - last + p);
            for (int j = 1; j <= last_k; ++j) { g[j] = f[j]; }
            for (int j = 1; j <= k; ++j) { f[j] = tmp[j]; }
        }
        last_delta = delta;
        last = i;
    }
    f.resize(k + 1);
    return f;
}
