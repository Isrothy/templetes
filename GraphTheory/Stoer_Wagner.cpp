auto stoer_wagner(std::vector<std::vector<int>> adj) {
    auto n = adj.size();
    std::vector<bool> del(n);
    int res = INT_MAX;
    for (int i = 1; i < n; ++i) {
        std::vector<int> w(n);
        std::vector<bool> vis(n);
        int s = -1, t = -1;
        for (int j = 0; j < n - i + 1; ++j) {
            int v = -1;
            for (int u = 0; u < n; ++u) {
                if (!del[u] && !vis[u] && (v == -1 || w[v] < w[u])) { v = u; }
            }
            vis[v] = true;
            for (int u = 0; u < n; ++u) {
                if (!del[u] && !vis[u]) { w[u] += adj[u][v]; }
            }
            s = t; t = v;
        }
        res = std::min(res, w[t]);
        del[t] = true;
        for (int u = 0; u < n; ++u) {
            adj[u][s] += adj[u][t];
            adj[s][u] += adj[t][u];
        }
    }
    return res;
}
