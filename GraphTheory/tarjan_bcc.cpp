struct TarjanBcc {
    std::vector<std::vector<int>> adj, bccs;
    std::vector<int> dfn, low;
    std::vector<bool> is_cut;
    std::stack<int> stk;
    int dfs_clock;
    TarjanBcc(std::span<std::pair<int, int>> edges, int n) : adj(n), dfn(n), low(n), is_cut(n), dfs_clock(0) {
        for (auto [u, v]: edges) {
            adj[u].emplace_back(v);
            adj[v].emplace_back(u);
        }
        for (int u = 0; u < n; ++u) {
            if (!dfn[u]) {
                auto tmp = dfs_clock;
                find_bccs(u, -1);
                if (dfs_clock - tmp == 1) { bccs.emplace_back(1, u); }
            }
        }
    }
private:
    void find_bccs(int u, int fa) {
        dfn[u] = low[u] = ++dfs_clock;
        stk.push(u);
        int child = 0;
        for (auto v: adj[u]) {
            if (v == fa) { continue; }
            if (!dfn[v]) {
                ++child;
                find_bccs(v, u);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    is_cut[u] = true;
                    bccs.emplace_back();
                    while (true) {
                        auto x = stk.top();
                        stk.pop();
                        bccs.back().push_back(x);
                        if (x == v) { break; }
                    }
                }
            } else {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (fa == -1) { is_cut[u] = child > 1; }
    }
};
