struct HopcroftKarp {
    std::vector<std::vector<int>> adj;
    std::vector<int> dx, dy, s, t;
    int nl, nr;
    HopcroftKarp(int nl, int nr) : adj(nl + 1), dx(nl + 1), dy(nr + 1), s(nl + 1), t(nr + 1), nl(nl), nr(nr) {}
    void add_edge(int u, int v) { adj[u].push_back(v); }
    bool bfs() {
        fill(dx.begin(), dx.end(), -1);
        fill(dy.begin(), dy.end(), -1);
        int ret = -1;
        std::queue<int> q;
        for (int u = 1; u <= nl; ++u) {
            if (!s[u]) {
                dx[u] = 0;
                q.push(u);
            }
        }
        while (!q.empty()) {
            auto u = q.front();
            q.pop();
            if (ret != -1 && dx[u] > ret) { break; }
            for (auto v: adj[u]) {
                if (dy[v] == -1) {
                    dy[v] = dx[u];
                    if (t[v]) {
                        dx[t[v]] = dy[v] + 1;
                        q.push(t[v]);
                    } else {
                        ret = dx[u];
                    }
                }
            }
        }
        return ret != -1;
    }
    bool dfs(int u) {
        for (auto v: adj[u]) {
            if (dy[v] == dx[u]) {
                dy[v] = -1;
                if (!t[v] || dfs(t[v])) {
                    s[u] = v;
                    t[v] = u;
                    return true;
                }
            }
        }
        return false;
    }
    int maximum_matching() {
        int res = 0;
        while (bfs()) {
            for (int u = 1; u <= nl; ++u) {
                if (!s[u] && dfs(u)) { ++res; }
            }
        }
        return res;
    }
};
