struct BlockCutTree {
    std::vector<std::pair<int, int>> edges;
    std::vector<std::vector<int>> adj;
    std::vector<int> dfn, low;
    std::stack<int> stk;
    int n, dfs_clock, bcc_cnt;
    BlockCutTree(std::span<std::pair<int, int>> edges, int n) : adj(n), dfn(n), low(n), n(n), dfs_clock(0), bcc_cnt(0) {
        for (auto [u, v]: edges) {
            adj[u].emplace_back(v);
            adj[v].emplace_back(u);
        }
        for (int u = 0; u < n; ++u) {
            if (!dfn[u]) { build(u, -1); }
        }
    }
private:
    void build(int u, int fa) {
        dfn[u] = low[u] = ++dfs_clock;
        stk.push(u);
        for (auto v: adj[u]) {
            if (v == fa) { continue; }
            if (!dfn[v]) {
                build(v, u);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    while (true) {
                        auto x = stk.top();
                        stk.pop();
                        edges.emplace_back(bcc_cnt + n, x);
                        if (x == v) { break; }
                    }
                    edges.emplace_back(u, bcc_cnt + n);
                    ++bcc_cnt;
                }
            } else {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
    }
};
