constexpr double Eps = 1e-10;
constexpr int sign(double x) { return (x > Eps) - (x < -Eps); }
void pivot(std::vector<std::vector<double>> &A, int l, int e, std::vector<int> &id) {
    auto m = (int) A.size();
    auto n = (int) A[0].size();
    double tmp = -1 / A[l][e];
    A[l][e] = -1;
    for (auto &x: A[l]) { x *= tmp; }
    for (int i = 0; i < m; ++i) {
        if (i == l) { continue; }
        tmp = A[i][e];
        A[i][e] = 0;
        for (int j = 0; j < n; ++j) { A[i][j] += tmp * A[l][j]; }
    }
    std::swap(id[e], id[l + n - 1]);
}
enum class SimplexResult {
    unbounded,
    infeasible,
    optimal,
};
auto simplex(const std::vector<std::vector<double>> &A, const std::vector<double> &b, const std::vector<double> &c) -> std::pair<SimplexResult, std::optional<std::vector<double>>> {
    auto m = static_cast<int>(A.size());
    auto n = static_cast<int>(A[0].size());
    std::vector<int> Id(n + m);
    for (int i = 0; i < n + m; ++i) { Id[i] = i; }
    std::vector<std::vector<double>> T(m + 1, std::vector<double>(n + 1));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) { T[i][j] = -A[i][j]; }
        T[i][n] = b[i];
    }
    for (int i = 0; i < n; ++i) { T[m][i] = c[i]; }
    while (true) {
        int l = 0;
        for (int i = 1; i < m; ++i) {
            if (T[i][n] < T[l][n]) { l = i; }
        }
        if (sign(T[l][n]) >= 0) { break; }
        int e = -1;
        for (int i = 0; i < n; ++i) {
            if (sign(T[l][i]) > 0 && (e == -1 || Id[i] > Id[e])) { e = i; }
        }
        if (e == -1) { return {SimplexResult::infeasible, std::nullopt}; }
        pivot(T, l, e, Id);
    }
    while (true) {
        int e = 0;
        for (int i = 1; i < n; ++i) {
            if (T[m][i] > T[m][e]) { e = i; }
        }
        if (sign(T[m][e]) <= 0) { break; }
        int l = -1;
        double tmp = -1;
        for (int i = 0; i < m; ++i) {
            if (sign(T[i][e]) < 0) {
                double x = -T[i][n] / T[i][e];
                if (l == -1 || sign(x - tmp) < 0 || (sign(x - tmp) == 0 && Id[i] > Id[l])) {
                    tmp = x;
                    l = i;
                }
            }
        }
        if (l == -1) { return {SimplexResult::unbounded, std::nullopt}; }
        pivot(T, l, e, Id);
    }
    auto result = std::vector<double>(n + 1);
    result[n] = T[m][n];
    for (int i = 0; i < m; ++i) {
        if (Id[n + i] < n) { result[Id[n + i]] = T[i][n]; }
    }
    return {SimplexResult::optimal, result};
}
