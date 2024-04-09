template<size_t M, size_t Sigma> struct ACAutomaton {
    int next[M][Sigma]{}, fail[M]{}, queue[M]{}, cnt[M]{};
    int n;
    ACAutomaton() { n = 1; }
    void insert(std::string_view s) {
        int p = 1;
        for (auto c: s) {
            auto &q = next[p][c - 'a'];
            if (!q) { q = ++n; }
            p = q;
        }
    }
    void build() {
        int head = 0, tail = 0;
        fail[1] = 1;
        for (auto &p: next[1]) {
            if (p) {
                fail[p] = 1;
                queue[tail++] = p;
            } else {
                p = 1;
            }
        }
        while (head < tail) {
            auto p = queue[head++];
            for (int i = 0; i < Sigma; ++i) {
                if (auto &q = next[p][i]) {
                    fail[q] = next[fail[p]][i];
                    queue[tail++] = q;
                } else {
                    q = next[fail[p]][i];
                }
            }
        }
    }
};
