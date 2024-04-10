template<int64_t mod> struct Matrix : private std::vector<std::vector<int64_t>> {
    int n{}, m{};
    Matrix() = default;
    using std::vector<std::vector<int64_t>>::vector;
    using std::vector<std::vector<int64_t>>::operator[];
    explicit Matrix(std::vector<std::vector<int64_t>> v) : std::vector<std::vector<int64_t>>(v) {
        n = v.size();
        m = v[0].size();
    }
    Matrix(int n, int m) : n(n), m(m) {
        resize(n);
        for (int i = 0; i < n; ++i) { (*this)[i].resize(m); }
    }
    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) { res[i][i] = 1; }
        return res;
    }
    static Matrix zero(int n) { return {n, n}; }
};
template<int64_t mod> auto augment(Matrix<mod> mat, const std::vector<int64_t> &v) {
    assert(mat.n == v.size());
    for (int i = 0; i < mat.n; ++i) { mat[i].push_back(v[i]); }
    return mat;
}
template<int64_t mod> auto augment(Matrix<mod> lhs, const Matrix<mod> &rhs) {
    assert(lhs.n == rhs.n);
    for (int i = 0; i < lhs.n; ++i) {
        lhs[i].reserve(lhs.m + rhs.m);
        for (auto x: rhs[i]) { lhs[i].push_back(x); }
    }
    return lhs;
}
template<int64_t mod> std::optional<Matrix<mod>> inverse(Matrix<mod> mat) {
    auto n = mat.n, m = mat.m;
    assert(n == m);
    mat = augment(std::move(mat), Matrix<mod>::identity(n));
    for (int i = 0; i < n; ++i) {
        if (!mat[i][i]) {
            for (int j = i + 1; j < n; ++j) {
                if (mat[j][i]) {
                    std::swap(mat[i], mat[j]);
                    break;
                }
            }
            if (mat[i][i] == 0) { return std::nullopt; }
        }
        int64_t t = inverse(mat[i][i], mod);
        for (int j = i; j < 2 * n; ++j) { mat[i][j] = mat[i][j] * t % mod; }
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int64_t s = mat[j][i];
                for (int k = i; k < 2 * n; ++k) { mat[j][k] = (mat[j][k] - mat[i][k] * s) % mod; }
            }
        }
    }
    Matrix<mod> res(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) { res[i][j] = mat[i][j + n]; }
    }
    return res;
}
template<int64_t mod> std::vector<std::vector<int64_t>> gaussian_elimination(Matrix<mod> mat, const std::vector<int64_t> &v) {
    auto n = mat.n, m = mat.m;
    assert(n == v.size());
    std::vector<int> p(n, -1), free;
    mat = augment(std::move(mat), v);
    int pivot = 0;
    for (int i = 0; i < n; ++i) {
        while (pivot < m && mat[i][pivot] == 0) {
            for (int j = i + 1; j < n; ++j) {
                if (mat[j][pivot]) {
                    std::swap(mat[i], mat[j]);
                    break;
                }
            }
            if (!mat[i][pivot]) {
                free.push_back(pivot);
                ++pivot;
            }
        }
        if (pivot == m) { break; }
        auto t = inverse(mat[i][pivot], mod);
        for (int j = pivot; j <= m; ++j) { mat[i][j] = mat[i][j] * t % mod; }
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int64_t s = mat[j][pivot];
                for (int k = pivot; k <= m; ++k) { mat[j][k] = (mat[j][k] - mat[i][k] * s) % mod; }
            }
        }
        p[i] = pivot++;
    }
    while (pivot < m) {
        free.push_back(pivot);
        ++pivot;
    }
    std::vector<int64_t> v0(m);
    for (int i = 0; i < n; ++i) {
        if (p[i] == -1) {
            if (mat[i][m]) { return {}; }
        } else {
            v0[p[i]] = mat[i][m];
        }
    }
    std::vector<std::vector<int64_t>> res;
    res.emplace_back(std::move(v0));
    for (auto i: free) {
        std::vector<int64_t> v(m);
        v[i] = 1;
        for (int j = 0; j < n; ++j) {
            if (p[j] != -1) { v[p[j]] = -mat[j][i]; }
        }
        res.emplace_back(std::move(v));
    }
    return res;
}
template<int64_t mod> auto determinant(Matrix<mod> mat) {
    auto n = mat.n;
    int64_t ret = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            while (mat[j][i]) {
                auto t = mat[i][i] / mat[j][i];
                for (int k = i; k < n; ++k) { mat[i][k] = (mat[i][k] - mat[j][k] * t) % mod; }
                std::swap(mat[i], mat[j]);
                ret = -ret;
            }
        }
        ret = ret * mat[i][i] % mod;
    }
    return ret;
}
