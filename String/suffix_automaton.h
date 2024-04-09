template<int M, int Sigma> struct SuffixAutomaton {
    int trans[2 * M][Sigma]{}, mxlen[2 * M]{}, slink[2 * M]{};
    int n{};
    SuffixAutomaton() : n(1) {}
    auto extend(int p, int c) {
        auto q = ++n;
        mxlen[q] = mxlen[p] + 1;
        memset(trans[q], 0, sizeof trans[q]);
        while (p && !trans[p][c]) {
            trans[p][c] = q;
            p = slink[p];
        }
        if (!p) {
            slink[q] = 1;
        } else {
            if (auto r = trans[p][c]; mxlen[r] == mxlen[p] + 1) {
                slink[q] = r;
            } else {
                auto o = ++n;
                slink[o] = slink[r];
                mxlen[o] = mxlen[p] + 1;
                memcpy(trans[o], trans[r], sizeof trans[o]);
                while (trans[p][c] == r) {
                    trans[p][c] = o;
                    p = slink[p];
                }
                slink[q] = slink[r] = o;
            }
        }
        return q;
    }
};
