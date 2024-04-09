auto eulerian_circle(std::span<std::pair<int, int>> edges, int n, bool directed) -> std::optional<std::vector<std::pair<int, bool>>> {
    if (edges.empty()) { return std::vector<std::pair<int, bool>>(); }
    std::vector<int> degree(n + 1);
    std::vector<std::vector<std::tuple<int, int, bool>>> adj(n + 1);
    for (int i = 0; i < edges.size(); ++i) {
        auto [u, v] = edges[i];
        --degree[u];
        ++degree[v];
        adj[u].emplace_back(v, i, 0);
        if (!directed) { adj[v].emplace_back(u, i, 1); }
    }
    int u = 0;
    for (int i = 1; i <= n; ++i) {
        if (directed ? degree[i] : degree[i] & 1) { return std::nullopt; }
        if (!u && !adj[i].empty()) { u = i; }
    }
    std::stack<std::tuple<int, int, bool>> stk;
    std::vector<int> cur(n + 1);
    std::vector<bool> vis(edges.size());
    std::vector<std::pair<int, bool>> path;
    stk.emplace(u, 0, false);
    while (!stk.empty()) {
        auto [u, e, dir] = stk.top();
        auto &i = cur[u];
        while (i < adj[u].size() && vis[std::get<1>(adj[u][i])]) { ++i; }
        if (i < adj[u].size()) {
            stk.push(adj[u][i]);
            vis[std::get<1>(adj[u][i])] = true;
            ++i;
        } else {
            path.emplace_back(e, dir);
            stk.pop();
        }
    }
    path.pop_back();
    reverse(path.begin(), path.end());
    return path.size() == edges.size() ? std::optional(path) : std::nullopt;
}
