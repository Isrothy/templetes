template<int M> struct LiChaoSegmentTree {
    struct Segment {
        long long x1, y1, x2, y2;
        int id;
    };
    Segment S[4 * M];
    bool cmp(const Segment &a, const Segment &b, long long x0) {
        auto h1 = (b.x2 - b.x1) * (x0 - a.x1) * (a.y2 - a.y1) + a.y1 * (a.x2 - a.x1) * (b.x2 - b.x1);
        auto h2 = (a.x2 - a.x1) * (x0 - b.x1) * (b.y2 - b.y1) + b.y1 * (b.x2 - b.x1) * (a.x2 - a.x1);
        return h1 == h2 ? a.id > b.id : h1 < h2;
    }
    void update(int p, int l, int r, int a, int b, const Segment &L) {
        int mid = (l + r) >> 1;
        if (l == a && r == b) {
            if (S[p].id == 0) {
                S[p] = L;
            } else {
                if (cmp(S[p], L, mid)) { swap(S[p], L); }
                if (l != r) {
                    if (cmp(S[p], L, l)) { update(p << 1, l, mid, a, mid, L); }
                    if (cmp(S[p], L, r)) { update(p << 1 | 1, mid + 1, r, mid + 1, b, L); }
                }
            }
            return;
        }
        if (b <= mid) {
            update(p << 1, l, mid, a, b, L);
        } else if (mid < a) {
            update(p << 1 | 1, mid + 1, r, a, b, L);
        } else {
            update(p << 1, l, mid, a, mid, L);
            update(p << 1 | 1, mid + 1, r, mid + 1, b, L);
        }
    }
    Segment query(int p, int l, int r, long long x) {
        if (l == r) { return S[p]; }
        int mid = (l + r) >> 1;
        Segment res{};
        if (x <= mid) {
            res = query(p << 1, l, mid, x);
        } else {
            res = query(p << 1 | 1, mid + 1, r, x);
        }
        if (S[p].id && (res.id == 0 || cmp(res, S[p], x))) { res = S[p]; }
        return res;
    }
};
