template<size_t M, size_t Sigma> struct PalindromicAutomaton {
    size_t next[M][Sigma]{}, fail[M]{};
    int length[M]{};
    std::deque<char> str;
    size_t n, left, right;
    PalindromicAutomaton() : n(2), left(0), right(0) {
        length[1] = -1;
        fail[0] = 1;
    }
    void push_back(int c) {
        auto get_fail = [&](size_t p) {
            while (str.size() <= length[p] + 1 || str[str.size() - length[p] - 2] != str.back()) { p = fail[p]; }
            return p;
        };
        str.push_back(c);
        auto p = get_fail(right);
        auto &q = next[p][c];
        if (!q) {
            auto r = ++n;
            length[r] = length[p] + 2;
            fail[r] = next[get_fail(fail[p])][c];
            q = r;
        }
        right = q;
        if (length[right] == str.size()) { left = right; }
    }
    void push_front(char c) {
        auto get_fail = [&](size_t p) {
            while (str.size() <= length[p] + 1 || str[length[p] + 1] != str.front()) { p = fail[p]; }
            return p;
        };
        str.push_front(c);
        auto p = get_fail(left);
        auto &q = next[p][c];
        if (!q) {
            auto r = ++n;
            length[r] = length[p] + 2;
            fail[r] = next[get_fail(fail[p])][c];
            q = r;
        }
        left = q;
        if (length[left] == str.size()) { right = left; }
    }
};
