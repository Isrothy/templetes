struct TarjanEcc {
    std::vector<std::vector<std::pair<int, int>>> adj;
    std::vector<int> dfn, low;
    std::vector<bool> is_bridge;
    std::vector<std::vector<int>> eccs;
    int dfs_clock;
    TarjanEcc(std::span<std::pair<int, int>> edges, int n) : adj(n), dfn(n), low(n), is_bridge(edges.size(), false), eccs(), dfs_clock(0) {
        for (int i = 0; i < edges.size(); ++i) {
            auto [u, v] = edges[i];
            adj[u].emplace_back(v, i);
            adj[v].emplace_back(u, i);
        }
        for (int u = 0; u < n; ++u) {
            if (!dfn[u]) { find_bridges(u, -1); }
        }
        std::vector<bool> visited(n);
        for (int u = 0; u < n; ++u) {
            if (!visited[u]) {
                std::vector<int> q;
                q.emplace_back(u);
                visited[u] = true;
                for (int ptr = 0; ptr < q.size(); ++ptr) {
                    auto v = q[ptr];
                    for (auto [w, id]: adj[v]) {
                        if (!is_bridge[id] && !visited[w]) {
                            visited[w] = true;
                            q.push_back(w);
                        }
                    }
                }
                eccs.emplace_back(std::move(q));
            }
        }
    }
private:
    void find_bridges(int u, int prev) {
        dfn[u] = low[u] = ++dfs_clock;
        for (auto [v, id]: adj[u]) {
            if (prev == id) { continue; }
            if (!dfn[v]) {
                find_bridges(v, id);
                low[u] = std::min(low[u], low[v]);
                is_bridge[id] = dfn[u] < low[v];
            } else {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
    }
};
