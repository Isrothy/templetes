#include <queue>
#include <vector>
struct Blossom {
    std::vector<std::vector<int>> adj;
    std::vector<int> mate, link, label, last_time, fa;
    int time = 0, n;
    explicit Blossom(int n) : adj(n + 1), mate(n + 1), link(n + 1), label(n + 1), last_time(n + 1), fa(n + 1), n(n) {}
    int find(int x) {
        if (x == fa[x]) { return x; }
        fa[x] = find(fa[x]);
        return fa[x];
    }
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int LCA(int u, int v) {
        ++time;
        while (last_time[u] != time) {
            if (u) {
                last_time[u] = time;
                u = find(link[mate[u]]);
            }
            std::swap(u, v);
        }
        return u;
    }
    void blossom(int u, int v, int w, std::queue<int> &q) {
        while (find(u) != w) {
            link[u] = v;
            v = mate[u];
            fa[v] = fa[u] = w;
            if (label[v] == 1) {
                label[v] = 2;
                q.push(v);
            }
            u = link[v];
        }
    }
    bool bfs(int s) {
        std::queue<int> q;
        for (int i = 1; i <= n; ++i) {
            fa[i] = i;
            label[i] = 0;
        }
        q.push(s);
        label[s] = 2;
        while (!q.empty()) {
            auto u = q.front();
            q.pop();
            for (auto v: adj[u]) {
                if (label[v] == 0) {
                    label[v] = 1;
                    link[v] = u;
                    if (mate[v] == 0) {
                        while (u) {
                            u = mate[link[v]];
                            mate[v] = link[v];
                            mate[mate[v]] = v;
                            v = u;
                        }
                        return true;
                    } else {
                        q.push(mate[v]);
                        label[mate[v]] = 2;
                    }
                } else if (label[v] == 2 && find(v) != find(u)) {
                    int w = LCA(u, v);
                    blossom(u, v, w, q);
                    blossom(v, u, w, q);
                }
            }
        }
        return false;
    }
    int maximum_matching() {
        int res = 0;
        for (int u = 1; u <= n; ++u) {
            if (!mate[u] && bfs(u)) { ++res; }
        }
        return res;
    }
};
