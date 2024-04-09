struct line {
    mutable ll m, b, p;
    bool operator<(const line &l) const { return m < l.m; }
    bool operator<(ll x) const { return p < x; }
    ll operator()(ll x) { return m * x + b; }
};
struct CHT : multiset<line, less<>> {
    // (for doubles, use oo = 1/.0, div(a,b) = a/b)
    static const ll oo = LLONG_MAX;
    ll div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }// floored
    // updates x->p, determines if y is useless
    bool isect(iterator x, iterator y) {
        if (y == end()) {
            x->p = oo;
            return false;
        }
        if (x->m == y->m) x->p = x->b > y->b ? oo : -oo;
        else x->p = div(y->b - x->b, x->m - y->m);
        return x->p >= y->p;
    }
    void add(ll m, ll b) {
        auto z = insert({m, b, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    ll qry(ll x) {
        if (empty()) throw;
        auto l = *lower_bound(x);
        return l(x);
    }
};

