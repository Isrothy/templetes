auto johnson(std::span<std::tuple<int, int, int>> edges, int n) -> std::optional<std::vector<std::vector<int>>> {
    std::vector<int> h(n, 0);
    for (int i = 0;; ++i) {
        bool flag = false;
        for (const auto &[u, v, w]: edges) {
            if (h[u] + w < h[v]) {
                h[v] = h[u] + w;
                flag = true;
            }
        }
        if (!flag) { break; }
        if (i == n) { return std::nullopt; }
    }
    std::vector<std::vector<int>> dis(n, std::vector<int>(n, INT_MAX));
    std::vector<std::vector<std::pair<int, int>>> adj(n);
    for (const auto &[u, v, w]: edges) { adj[u].emplace_back(v, w); }
    for (int i = 0; i < n; ++i) {
        std::priority_queue<std::pair<int, int>> heap;
        dis[i][i] = 0;
        heap.emplace(0, i);
        while (!heap.empty()) {
            auto [d, u] = heap.top();
            heap.pop();
            if (dis[i][u] != -d) { continue; }
            for (auto [v, w]: adj[u]) {
                if (auto d = dis[i][u] + h[u] - h[v] + w; d < dis[i][v]) {
                    dis[i][v] = d;
                    heap.emplace(-d, v);
                }
            }
        }
        for (int j = 0; j < n; ++j) {
            if (dis[i][j] != INT_MAX) { dis[i][j] += h[j] - h[i]; }
        }
    }
    return dis;
}
