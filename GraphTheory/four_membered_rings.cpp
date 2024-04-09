auto four_membered_rings(std::span<std::vector<size_t>> adj) {
    auto n = adj.size();
    std::vector<size_t> rank(n), vis_time(n, -1), cnt(n);
    std::vector<std::vector<size_t>> f(n);
    for (int u = 0; u < n; ++u) { rank[u] = adj[u].size() * n + u; }
    for (int u = 0; u < n; ++u) {
        for (auto v: adj[u]) {
            if (rank[u] < rank[v]) { f[u].push_back(v); }
        }
    }
    size_t res = 0;
    for (size_t u = 0; u < n; ++u) {
        for (auto v: adj[u]) {
            for (auto w: f[v]) {
                if (rank[u] < rank[w]) {
                    if (vis_time[w] < u) {
                        vis_time[w] = u;
                        cnt[w] = 0;
                    }
                    res += cnt[w];
                    ++cnt[w];
                }
            }
        }
    }
    return res;
}
