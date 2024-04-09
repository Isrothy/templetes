enum class SuffixType : bool { l, s };
bool is_lms_char(std::span<SuffixType> type, int x) { return x > 0 && type[x] == SuffixType::s && type[x - 1] == SuffixType::l; }
bool substring_equal(std::span<int> s, std::span<SuffixType> type, int x, int y) {
    do {
        if (s[x] != s[y]) { return false; }
        ++x;
        ++y;
    } while (!is_lms_char(type, x) && !is_lms_char(type, y));
    return s[x] == s[y];
}
void induce_sort(std::span<int> s, std::vector<int> &sa, std::span<SuffixType> type, std::vector<int> &bucket, std::vector<int> &lbucket, std::vector<int> &sbucket, size_t n, size_t sigma) {
    lbucket[0] = sbucket[0] = 1;
    for (int i = 1; i < sigma; ++i) {
        lbucket[i] = bucket[i - 1];
        sbucket[i] = bucket[i];
    }
    for (int i = 0; i < n; ++i) {
        if (sa[i] > 0 && type[sa[i] - 1] == SuffixType::l) { sa[lbucket[s[sa[i] - 1]]++] = sa[i] - 1; }
    }
    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
        if (sa[i] > 0 && type[sa[i] - 1] == SuffixType::s) { sa[--sbucket[s[sa[i] - 1]]] = sa[i] - 1; }
    }
}
std::vector<int> SA_IS(std::span<int> s, size_t sigma) {
    size_t n = s.size();
    std::vector<SuffixType> type(n);
    std::vector<int> bucket(sigma), lbucket(sigma), sbucket(sigma);
    for (int i = 0; i < n; i++) { bucket[s[i]]++; }
    std::partial_sum(bucket.begin(), bucket.end(), bucket.begin());
    type[n - 1] = SuffixType::s;
    for (int i = static_cast<int>(n) - 2; i >= 0; i--) { type[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && type[i + 1] == SuffixType::s) ? SuffixType::s : SuffixType::l; }
    std::vector<int> position;
    for (int i = 1; i < n; ++i) {
        if (type[i] == SuffixType::s && type[i - 1] == SuffixType::l) { position.push_back(i); }
    }
    auto m = position.size();
    sbucket[0] = 1;
    for (int i = 1; i < sigma; ++i) { sbucket[i] = bucket[i]; }
    std::vector<int> sa(n, -1);
    for (int i = 0; i < m; i++) { sa[--sbucket[s[position[i]]]] = position[i]; }
    induce_sort(s, sa, type, bucket, lbucket, sbucket, n, sigma);
    std::vector<int> name(n, -1);
    int last = -1, name_cnt = 1;
    bool duplicated = false;
    for (int i = 0; i < n; i++) {
        int x = sa[i];
        if (is_lms_char(type, x)) {
            if (last != -1) {
                if (!substring_equal(s, type, x, last)) {
                    ++name_cnt;
                } else {
                    duplicated = true;
                }
            }
            name[x] = name_cnt - 1;
            last = x;
        }
    }
    std::vector<int> s1;
    s1.reserve(m);
    for (int i = 0; i < n; ++i) {
        if (name[i] >= 0) { s1.push_back(name[i]); }
    }
    std::vector<int> sa1;
    if (!duplicated) {
        sa1.resize(m);
        for (int i = 0; i < m; ++i) { sa1[s1[i]] = i; }
    } else {
        sa1 = SA_IS(s1, name_cnt);
    }
    sbucket[0] = 1;
    for (int i = 1; i < sigma; ++i) { sbucket[i] = bucket[i]; }
    std::fill(sa.begin(), sa.end(), -1);
    for (int i = (int) m - 1; i >= 0; --i) { sa[--sbucket[s[position[sa1[i]]]]] = position[sa1[i]]; }
    induce_sort(s, sa, type, bucket, lbucket, sbucket, n, sigma);
    return sa;
}
auto suffix_sort(std::string_view str) {
    auto n = str.size();
    std::vector<int> s(n + 1);
    for (int i = 0; i < n; ++i) { s[i] = static_cast<unsigned char>(str[i]); }
    auto sa = SA_IS(s, 128);
    std::vector<int> rank(n + 1), height(n);
    for (int i = 0; i <= n; ++i) { rank[sa[i]] = i; }
    for (int i = 0, h = 0; i < n; ++i) {
        if (h) { --h; }
        int j = sa[rank[i] - 1];
        while (i + h < n && j + h < n && str[i + h] == str[j + h]) { ++h; }
        height[rank[i] - 1] = h;
    }
    return std::make_pair(sa, height);
}
