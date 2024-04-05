#include <optional>
#include <stack>
#include <vector>
struct TwoSAT {
    std::stack<int> stk;
    std::vector<int> sccno, dfn, low;
    std::vector<std::vector<int>> adj;
    std::vector<bool> on_stack;
    int n, scc_cnt, idx;
    explicit TwoSAT(int n) : sccno(2 * n), dfn(2 * n), low(2 * n), adj(2 * n), on_stack(2 * n), n(n), scc_cnt(0), idx(0) {}
    void add_clause(int x, int xval, int y, int yval) {
        x = x * 2 + xval;
        y = y * 2 + yval;
        adj[x ^ 1].push_back(y);
        adj[y ^ 1].push_back(x);
    }
    void Tarjan(int u) {
        dfn[u] = low[u] = ++idx;
        stk.push(u);
        on_stack[u] = true;
        for (auto v: adj[u]) {
            if (!dfn[v]) {
                Tarjan(v);
                low[u] = std::min(low[u], low[v]);
            } else if (on_stack[v]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            ++scc_cnt;
            while (true) {
                auto x = stk.top();
                stk.pop();
                on_stack[x] = false;
                sccno[x] = scc_cnt;
                if (x == u) { break; }
            }
        }
    }
    auto solve() -> std::optional<std::vector<bool>> {
        for (int i = 0; i < 2 * n; ++i) {
            if (!dfn[i]) { Tarjan(i); }
        }
        std::vector<bool> res(n);
        for (int i = 0; i < n; ++i) {
            if (sccno[i * 2] == sccno[i * 2 + 1]) { return std::nullopt; }
            res[i] = sccno[i * 2] > sccno[i * 2 + 1];
        }
        return res;
    }
};
