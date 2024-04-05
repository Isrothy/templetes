#include <span>
#include <stack>
struct TarjanScc {
    std::vector<std::vector<int>> adj, sccs;
    std::vector<int> dfn, low;
    std::stack<int> stk;
    std::vector<bool> on_stack;
    int dfs_clock;
    TarjanScc(std::span<std::pair<int, int>> edges, int n) : adj(n + 1), dfn(n + 1), low(n + 1), on_stack(n + 1), dfs_clock(0) {
        for (auto [u, v]: edges) { adj[u].emplace_back(v); }
        for (int u = 1; u <= n; ++u) {
            if (!dfn[u]) { find_sccs(u); }
        }
    }
private:
    void find_sccs(int u) {
        dfn[u] = low[u] = ++dfs_clock;
        stk.push(u);
        on_stack[u] = true;
        for (auto v: adj[u]) {
            if (!dfn[v]) {
                find_sccs(v);
                low[u] = std::min(low[u], low[v]);
            } else if (on_stack[v]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            sccs.emplace_back();
            while (true) {
                auto v = stk.top();
                stk.pop();
                on_stack[v] = false;
                sccs.back().push_back(v);
                if (v == u) { break; }
            }
        }
    }
};
