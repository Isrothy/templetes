#include <algorithm>
#include <bit>
#include <cassert>
#include <functional>
#include <span>
template<size_t M, typename T, typename Cmp = std::less<>> struct Sparse_Table {
    static constexpr int K = std::bit_floor(M) + 1;
    T ST[K][M];
    void init(std::span<T> a) {
        auto n = a.size();
        for (int i = 1; i <= n; ++i) { ST[0][i] = a[i]; }
        for (int k = 1; k < K; ++k) {
            for (int i = 1; i + (1 << k) - 1 <= n; ++i) { ST[k][i] = std::min(ST[k - 1][i], ST[k - 1][i + (1zu << (k - 1))], Cmp()); }
        }
    }
    T query(size_t l, size_t r) {
        auto k = std::bit_floor(r - l + 1);
        return std::min(ST[k][l], ST[k][r - (1 << k) + 1], Cmp());
    }
};
