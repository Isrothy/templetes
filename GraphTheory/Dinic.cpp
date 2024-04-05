#include <queue>
#include <vector>
struct Dinic {
    struct Edge {
        int from, to;
        int64_t cap, flow;
        Edge(int u, int v, int64_t c) : from(u), to(v), cap(c), flow(0) {}
    };
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;
    std::vector<int> dis, cur;
    std::vector<bool> vis;
    int n;
    explicit Dinic(int n) : adj(n + 1), dis(n + 1), cur(n + 1), vis(n + 1), n(n + 1) {}
    void add_edge(int u, int v, int64_t c) {
        adj[u].push_back((int) edges.size());
        edges.emplace_back(u, v, c);
        adj[v].push_back((int) edges.size());
        edges.emplace_back(v, u, 0);
    }
    bool bfs(int s, int t) {
        std::queue<int> q;
        std::fill(vis.begin(), vis.end(), false);
        dis[t] = 0;
        vis[t] = true;
        q.push(t);
        while (!q.empty()) {
            auto u = q.front();
            q.pop();
            for (auto i: adj[u]) {
                const auto &e = edges[i ^ 1];
                if (e.flow < e.cap && !vis[e.from]) {
                    vis[e.from] = true;
                    dis[e.from] = dis[u] + 1;
                    q.push(e.from);
                }
            }
        }
        return vis[s];
    }
    auto dfs(int u, int t, int64_t a) {
        if (u == t) { return a; }
        auto m = a;
        for (auto &i = cur[u]; i < adj[u].size(); ++i) {
            auto &e = edges[adj[u][i]];
            if (e.flow < e.cap && vis[e.to] && dis[e.to] == dis[u] - 1) {
                auto f = dfs(e.to, t, std::min(m, e.cap - e.flow));
                e.flow += f;
                edges[adj[u][i] ^ 1].flow -= f;
                m -= f;
                if (!m) { break; }
            }
        }
        return a - m;
    }
    auto max_flow(int s, int t) {
        int64_t flow = 0;
        while (bfs(s, t)) {
            std::fill(cur.begin(), cur.end(), 0);
            flow += dfs(s, t, std::numeric_limits<int64_t>::max());
        }
        return flow;
    }
};
