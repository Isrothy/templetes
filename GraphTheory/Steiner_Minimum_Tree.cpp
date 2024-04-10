auto Steiner_Minimum_Tree(std::span<std::vector<std::pair<int, int>>> adj, std::span<int> s) -> std::optional<std::vector<std::tuple<int, int, int>>> {
    static constexpr int INF = 0x3f3f3f3f;
    auto n = adj.size(), k = s.size();
    auto dp = std::vector(1 << k, std::vector<int>(n, INF));
    auto prev = std::vector(1 << k, std::vector<std::pair<int, int>>(n));
    for (int i = 0; i < k; ++i) {
        dp[1 << i][s[i]] = 0;
        prev[1 << i][s[i]] = {0, -1};
    }
    for (int S = 1; S < 1 << k; ++S) {
        for (int T = (S - 1) & S; T; --T &= S) {
            for (int u = 0; u < n; ++u) {
                if (auto val = dp[T][u] + dp[S ^ T][u]; val < dp[S][u]) {
                    dp[S][u] = val;
                    prev[S][u] = std::make_pair(1, T);
                }
            }
        }
        std::priority_queue<std::pair<int, int>> q;
        for (int u = 0; u < n; ++u) { q.emplace(-dp[S][u], u); }
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (-d != dp[S][u]) { continue; }
            for (auto &[v, w]: adj[u]) {
                if (auto val = -d + w; val < dp[S][v]) {
                    dp[S][v] = val;
                    prev[S][v] = std::make_pair(2, u);
                    q.emplace(-dp[S][v], v);
                }
            }
        }
    }
    auto min = std::min_element(dp[(1 << k) - 1].begin(), dp[(1 << k) - 1].end());
    if (*min == INF) { return std::nullopt; }
    std::vector<std::tuple<int, int, int>> edges;
    std::queue<std::pair<int, int>> q;
    q.emplace((1 << k) - 1, min - dp[(1 << k) - 1].begin());
    while (!q.empty()) {
        auto [S, u] = q.front();
        q.pop();
        switch (auto [x, y] = prev[S][u]; x) {
            case 1:
                q.emplace(y, u);
                q.emplace(S ^ y, u);
                break;
            case 2:
                q.emplace(S, y);
                edges.emplace_back(y, u, dp[S][u] - dp[S][y]);
                break;
            default: break;
        }
    }
    return edges;
}
