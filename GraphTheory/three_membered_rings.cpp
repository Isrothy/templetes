#include <span>
auto three_membered_rings(std::span<std::vector<int>> adj) {
    int n = (int) adj.size() - 1;
    std::vector<size_t> rank(n + 1);
    std::vector<int> vis_time(n + 1);
    std::vector<std::vector<int>> f(n + 1);
    for (int u = 1; u <= n; ++u) { rank[u] = adj[u].size() * (n + 1) + u; }
    for (int u = 1; u <= n; ++u) {
        for (auto v: adj[u]) {
            if (rank[u] < rank[v]) { f[u].push_back(v); }
        }
    }
    std::vector<std::tuple<int, int, int>> rings;
    for (int u = 1; u <= n; ++u) {
        for (auto v: f[u]) { vis_time[v] = u; }
        for (auto v: f[u]) {
            for (auto w: f[v]) {
                if (vis_time[w] == u) { rings.emplace_back(u, v, w); }
            }
        }
    }
    return rings;
}
