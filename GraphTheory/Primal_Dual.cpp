struct PrimalDual {
    static constexpr int64_t INF = 0x3f3f3f3f3f3f3f3f;
    struct Edge {
        int from, to;
        int64_t cap, cost, flow;
        Edge(int from, int to, int64_t cap, int64_t cost) : from(from), to(to), cap(cap), cost(cost), flow(0) {}
    };
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;
    std::vector<int64_t> dis, h;
    std::vector<bool> vis, in_queue;
    int n;
    explicit PrimalDual(int n) : adj(n), dis(n), h(n), vis(n), in_queue(n), n(n) {}
    void add_edge(int u, int v, int64_t cap, int64_t cost) {
        adj[u].push_back((int) edges.size());
        edges.emplace_back(u, v, cap, cost);
        adj[v].push_back((int) edges.size());
        edges.emplace_back(v, u, 0, -cost);
    }
    void spfa(int t) {
        std::queue<int> q;
        std::vector<bool> in_queue(n, false);
        std::fill(dis.begin(), dis.end(), INF);
        dis[t] = 0;
        in_queue[t] = true;
        q.push(t);
        while (!q.empty()) {
            auto u = q.front();
            q.pop();
            in_queue[u] = false;
            for (auto i: adj[u]) {
                const auto &e = edges[i ^ 1];
                if (e.flow != e.cap && dis[u] + e.cost < dis[e.from]) {
                    dis[e.from] = dis[u] + e.cost;
                    if (!in_queue[e.from]) {
                        in_queue[e.from] = true;
                        q.push(e.from);
                    }
                }
            }
        }
    }
    void dijkstra(int t) {
        std::priority_queue<std::pair<int, int>> q;
        std::fill(dis.begin(), dis.end(), INF);
        dis[t] = 0;
        q.emplace(0, t);
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (dis[u] != -d) { continue; }
            for (auto i: adj[u]) {
                const auto &e = edges[i ^ 1];
                auto c = dis[u] + e.cost + h[u] - h[e.from];
                if (e.flow < e.cap && c < dis[e.from]) {
                    dis[e.from] = c;
                    q.emplace(-c, e.from);
                }
            }
        }
    }
    auto dfs(int u, int t, int64_t a) {
        if (u == t) { return a; }
        vis[u] = true;
        auto m = a;
        for (auto i: adj[u]) {
            auto &e = edges[i];
            if (e.flow < e.cap && !vis[e.to] && h[e.to] == h[u] - e.cost) {
                auto f = dfs(e.to, t, std::min(m, e.cap - e.flow));
                e.flow += f;
                edges[i ^ 1].flow -= f;
                m -= f;
                if (m == 0) { break; }
            }
        }
        return a - m;
    }
    auto minimum_cost_flow(int s, int t) {
        int64_t flow = 0, cost = 0;
        for (spfa(t); dis[s] != INF; dijkstra(t)) {
            for (int i = 0; i < n; ++i) { h[i] += dis[i]; }
            while (true) {
                std::fill(vis.begin(), vis.end(), false);
                if (auto f = dfs(s, t, INF)) {
                    flow += f;
                    cost += f * h[s];
                } else {
                    break;
                }
            }
        }
        return std::make_pair(flow, cost);
    }
};
