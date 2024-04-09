template<size_t M, size_t Sigma> struct GeneralSuffixAutomaton {
    size_t trans[2 * M + 10][Sigma]{}, mxlen[2 * M + 10]{}, slink[2 * M + 10]{};
    size_t n;
    GeneralSuffixAutomaton() : n(1) {}
    auto extend(size_t p, int c) {
        if (auto r = trans[p][c]) {
            if (mxlen[r] == mxlen[p] + 1) { return r; }
            auto o = ++n;
            slink[o] = slink[r];
            mxlen[o] = mxlen[p] + 1;
            memcpy(trans[o], trans[r], sizeof trans[o]);
            while (trans[p][c] == r) {
                trans[p][c] = o;
                p = slink[p];
            }
            slink[r] = o;
            return o;
        }
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
