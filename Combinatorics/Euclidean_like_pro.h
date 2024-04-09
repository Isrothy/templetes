// sum_{x=0}^n x^{k1} ((ax+b)/c)^{k2}
template<int64_t M, int64_t Mod> class EuclideanLike {
    constexpr static int64_t M1 = M + 2;
    int64_t B[M1]{}, P[M1][M1]{}, C[M1][M1]{}, inv[M1]{};
    constexpr int64_t power_sum(int m, int64_t n) {
        n %= Mod;
        int64_t sum = 0, x = n;
        for (int i = m; i >= 0; --i) {
            sum = (sum + P[m][i] * x) % Mod;
            x = x * n % Mod;
        }
        return sum;
    }
public:
    EuclideanLike() {
        inv[1] = 1;
        for (int i = 2; i < M1; ++i) { inv[i] = -Mod / i * inv[Mod % i] % Mod; }
        for (int i = 0; i < M1; ++i) {
            C[i][0] = 1;
            for (int j = 1; j <= i; ++j) { C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % Mod; }
        }
        B[0] = 1;
        for (int i = 1; i < M1; ++i) {
            B[i] = 1;
            for (int j = 0; j < i; ++j) { B[i] = (B[i] - C[i][j] * B[j] % Mod * inv[i - j + 1]) % Mod; }
        }
        for (int i = 0; i < M1 - 1; ++i) {
            for (int j = 0; j <= i + 1; ++j) { P[i][j] = inv[i + 1] * C[i + 1][j] % Mod * B[j] % Mod; }
        }
    }
    auto evaluate(int64_t n, int64_t a, int64_t b, int64_t c, int K) -> std::vector<std::vector<int64_t>> {
        int64_t s[K + 1], power_x[K + 1], power_y[K + 1], power_m[K + 1];
        int64_t x = a / c % Mod, y = b / c % Mod;
        int64_t m = ((__int128) a * n + b) / c;
        for (int i = 0; i <= K; ++i) { s[i] = power_sum(i, n); }
        power_x[0] = power_y[0] = power_m[0] = 1;
        for (int i = 1; i <= K; ++i) {
            power_x[i] = power_x[i - 1] * x % Mod;
            power_y[i] = power_y[i - 1] * y % Mod;
            power_m[i] = power_m[i - 1] * (m % Mod) % Mod;
        }
        auto res = std::vector<std::vector<int64_t>>(K + 1);
        for (int i = 0; i <= K; ++i) { res[i].resize(K - i + 1); }
        if (!a) {
            for (int k2 = 0; k2 <= K; ++k2) {
                for (int k1 = 0; k1 <= K - k2; ++k1) { res[k1][k2] = power_y[k2] * (k1 == 0 ? (n + 1) : s[k1]) % Mod; }
            }
        } else if (a >= c) {
            auto tmp = evaluate(n, a % c, b, c, K);
            for (int k2 = 0; k2 <= K; ++k2) {
                for (int j = 0; j <= k2; ++j) {
                    int64_t u = C[k2][j] * power_x[j] % Mod;
                    for (int k1 = 0; k1 <= K - k2; ++k1) { res[k1][k2] = (res[k1][k2] + u * tmp[k1 + j][k2 - j]) % Mod; }
                }
            }
        } else if (b >= c) {
            auto tmp = evaluate(n, a, b % c, c, K);
            for (int k2 = 0; k2 <= K; ++k2) {
                for (int j = 0; j <= k2; ++j) {
                    int64_t u = C[k2][j] * power_y[j] % Mod;
                    for (int k1 = 0; k1 <= K - k2; ++k1) { res[k1][k2] = (res[k1][k2] + u * tmp[k1][k2 - j]) % Mod; }
                }
            }
        } else {
            auto tmp = evaluate(m - 1, c, c - b - 1, a, K), D = res;
            for (int k1 = 0; k1 <= K; ++k1) { res[k1][0] = (k1 == 0 ? (n + 1) : s[k1]) % Mod; }
            for (int i = 0; i <= K; ++i) {
                for (int j = 0; j <= K - i; ++j) {
                    for (int k = 0; k <= i; ++k) { D[i][j] = (D[i][j] + C[i + 1][k] * tmp[k][j]) % Mod; }
                }
            }
            for (int k2 = 1; k2 <= K; ++k2) {
                for (int k1 = 0; k1 <= K - k2; ++k1) {
                    res[k1][k2] = power_m[k2] * s[k1] % Mod;
                    for (int j = 0; j <= k1; ++j) { res[k1][k2] = (res[k1][k2] - P[k1][j] * D[k2 - 1][k1 + 1 - j]) % Mod; }
                }
            }
        }
        return res;
    }
};
