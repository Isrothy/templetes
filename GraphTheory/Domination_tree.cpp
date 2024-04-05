#include <span>
#include <vector>
struct DominatorTree {
    std::vector<std::vector<int>> adj, rev, bucket;
    std::vector<int> parent, dfn, va, dsu, idom, sdom, label;
    auto find(int u) {
        if (u == dsu[u]) { return u; }
        if (auto &v = dsu[u], w = find(v); v != w) {
            if (dfn[sdom[label[v]]] < dfn[sdom[label[u]]]) { label[u] = label[v]; }
            v = w;
        }
        return dsu[u];
    }
    void dfs(int u) {
        dfn[u] = (int) va.size();
        va.push_back(u);
        for (auto v: adj[u]) {
            if (dfn[v] == 0) {
                parent[v] = u;
                dfs(v);
            }
        }
    }
    DominatorTree(std::span<std::pair<int, int>> edges, int n, int root) : adj(n + 1), rev(n + 1), bucket(n + 1), parent(n + 1), dfn(n + 1), va(1), dsu(n + 1), idom(n + 1), sdom(n + 1), label(n + 1) {
        for (auto [u, v]: edges) {
            adj[u].push_back(v);
            rev[v].push_back(u);
        }
        dfs(root);
        for (int u = 1; u <= n; ++u) { dsu[u] = sdom[u] = label[u] = u; }
        for (auto i = (int) va.size() - 1; i; --i) {
            auto u = va[i];
            for (auto v: rev[u]) {
                if (dfn[v]) {
                    find(v);
                    sdom[u] = std::min(sdom[u], sdom[label[v]], [&](auto a, auto b) { return dfn[a] < dfn[b]; });
                }
            }
            for (auto v: bucket[u]) {
                find(v);
                idom[v] = sdom[v] == sdom[label[v]] ? sdom[v] : label[v];
            }
            if (i != 1) {
                dsu[u] = parent[u];
                bucket[sdom[u]].push_back(u);
            }
        }
        for (int i = 2; i < va.size(); ++i) {
            if (auto u = va[i]; idom[u] != sdom[u]) { idom[u] = idom[idom[u]]; }
        }
    }
};
