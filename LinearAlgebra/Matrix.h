template<int64_t mod> struct Matrix : private std::vector<std::vector<int64_t>> {
    int n{}, m{};
    Matrix() = default;
    using std::vector<std::vector<int64_t>>::vector;
    int64_t operator()(int i, int j) const { return (*this)[i][j]; }
    int64_t &operator()(int i, int j) { return (*this)[i][j]; }
    explicit Matrix(std::vector<std::vector<int64_t>> v) : std::vector<std::vector<int64_t>>(v) {
        n = v.size();
        m = v[0].size();
    }
    Matrix(int n, int m) : n(n), m(m) {
        resize(n);
        for (int i = 0; i < n; ++i) { (*this)[i].resize(m); }
    }
    Matrix augment(std::vector<int64_t> v) const {
        assert(n == v.size());
        Matrix res(n, m + 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) { res[i][j] = (*this)[i][j]; }
            res[i][m] = v[i];
        }
        return res;
    }
    std::vector<std::vector<int64_t>> gaussian_elimination(const std::vector<int64_t> &v) const {
        assert(n == v.size());
        std::vector<int64_t> v0(m);
        std::vector<int> p(n, -1), f;
        Matrix tmp = this->augment(v);
        for (int i = 0, pivot = 0; i < n; ++i) {
            while (pivot < m && tmp[i][pivot] == 0) {
                for (int j = i + 1; j < n; ++j) {
                    if (tmp[j][pivot]) {
                        std::swap(tmp[i], tmp[j]);
                        break;
                    }
                }
                if (!tmp[i][pivot]) {
                    f.push_back(pivot);
                    ++pivot;
                }
            }
            if (pivot == m) { break; }
            int64_t t = inv(tmp[i][pivot], mod);
            for (int j = pivot; j <= m; ++j) { tmp[i][j] = tmp[i][j] * t % mod; }
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    int64_t s = tmp[j][pivot];
                    for (int k = pivot; k <= m; ++k) { tmp[j][k] = (tmp[j][k] - tmp[i][k] * s) % mod; }
                }
            }
            p[i] = pivot++;
        }
        for (int i = 0; i < n; ++i) {
            if (p[i] == -1) {
                if (tmp[i][m]) { return {}; }
            } else {
                v0[p[i]] = tmp[i][m];
            }
        }
        std::vector<std::vector<int64_t>> res;
        res.push_back(v0);
        for (auto i: f) {
            std::vector<int64_t> v(m, 0);
            v[i] = 1;
            for (int j = 0; j < n; ++j) {
                if (i != j && p[j] != -1) { v[p[j]] = -tmp[j][i]; }
            }
            res.push_back(v);
        }
        return res;
    }
    std::optional<Matrix> inverse() const {
        assert(n == m);
        auto tmp = this->augment(Matrix::identity(n));
        for (int i = 0; i < n; ++i) {
            if (tmp[i][i] == 0) {
                for (int j = i + 1; j < n; ++j) {
                    if (tmp[j][i]) {
                        std::swap(tmp[i], tmp[j]);
                        break;
                    }
                }
                if (tmp[i][i] == 0) { return std::nullopt; }
            }
            int64_t t = inv(tmp[i][i], mod);
            for (int j = i; j < 2 * n; ++j) { tmp[i][j] = tmp[i][j] * t % mod; }
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    int64_t s = tmp[j][i];
                    for (int k = i; k < 2 * n; ++k) { tmp[j][k] = (tmp[j][k] - tmp[i][k] * s) % mod; }
                }
            }
        }
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) { res[i][j] = tmp[i][j + n]; }
        }
        return res;
    }
    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) { res[i][i] = 1; }
        return res;
    }
    static Matrix zero(int n) { return {n, n}; }
};
