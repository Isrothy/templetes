int minimal_representation(std::string_view s) {
    auto n = s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (s[(i + k) % n] == s[(j + k) % n]) {
            ++k;
        } else {
            if (s[(i + k) % n] < s[(j + k) % n]) {
                j += k + 1;
            } else {
                i += k + 1;
            }
            if (i == j) { ++i; }
            k = 0;
        }
    }
    return std::min(i, j);
}
