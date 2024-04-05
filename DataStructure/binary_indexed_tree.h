#include <cctype>
#include <span>
template<int M> struct binary_indexed_tree {
    long long b0[M], b1[M];
    int n;
    void update(int l, int r, int x) {
        for (auto i = l; i <= n; i += i & -i) {
            b0[i] -= (long long) (l - 1) * x;
            b1[i] += x;
        }
        for (auto i = r + 1; i <= n; i += i & -i) {
            b0[i] += (long long) r * x;
            b1[i] -= x;
        }
    }
    auto query(int i) {
        long long x = 0, y = 0;
        for (auto j = i; j; j -= j & -j) {
            x += b0[j];
            y += b1[j];
        }
        return x + y * i;
    }
    auto query(int l, int r) { return query(r) - query(l - 1); }
    void build(std::span<int> a) {
        auto n = a.size();
        this->n = n;
        for (int i = 1; i <= n; ++i) { b0[i] = b1[i] = 0; }
        for (int i = 1; i <= n; ++i) {
            b0[i] += a[i];
            if (i + (i & -i) < M) { b0[i + (i & -i)] += b0[i]; }
        }
    }
};
