struct MOD {
    ull b, m;
    MOD(ull _b) {
        b = _b;
        m = ull((INT(1) << 64) / b);
    }
    ull reduce(ull a) {
        ull q = (ull) ((INT(m) * a) >> 64);
        ull r = a - q * b;// proven theorem: 0 <= r < 2 * b
        return r >= b ? r - b : r;
    }
};
