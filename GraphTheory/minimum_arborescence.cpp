#include <optional>
#include <span>
#include <stack>
#include <vector>
template<class T> struct MinimumArborescence {
    struct Heap {
        T val{}, lazy{};
        int from{}, to{}, id{}, ch[2]{};
    };
    std::vector<Heap> heaps;
    std::vector<int> parent, h, mark;
    std::stack<int> stk;
    std::optional<std::vector<int>> choices;
    auto find(int p) {
        while (parent[p] >= 0) { p = parent[p]; }
        return p;
    }
    int unite(int p, int q) {
        p = find(p);
        q = find(q);
        if (p == q) { return p; }
        if (-parent[p] < -parent[q]) { std::swap(p, q); }
        parent[p] += parent[q];
        parent[q] = p;
        h[p] = merge(h[p], h[q]);
        stk.push(q);
        return p;
    }
    void rollback(int t) {
        while (stk.size() > t) {
            parent[stk.top()] = -1;
            stk.pop();
        }
    }
    void update(int p, T x) {
        heaps[p].val += x;
        heaps[p].lazy += x;
    }
    void push_down(int p) {
        auto &h = heaps[p];
        if (auto &x = h.lazy) {
            if (h.ch[0]) { update(h.ch[0], x); }
            if (h.ch[1]) { update(h.ch[1], x); }
            x = 0;
        }
    }
    auto merge(int p, int q) {
        if (!p || !q) { return p + q; }
        if (heaps[p].val > heaps[q].val) { std::swap(p, q); }
        push_down(p);
        heaps[p].ch[1] = merge(heaps[p].ch[1], q);
        std::swap(heaps[p].ch[0], heaps[p].ch[1]);
        return p;
    }
    auto pop(int p) {
        push_down(p);
        return merge(heaps[p].ch[0], heaps[p].ch[1]);
    }
    MinimumArborescence(std::span<std::tuple<int, int, T>> edges, int n, int root) : heaps(1), parent(n + 1), h(n + 1), mark(n + 1) {
        for (int i = 0; i < edges.size(); ++i) {
            auto [from, to, w] = edges[i];
            heaps.push_back({w, 0, from, to, i});
            h[to] = merge(h[to], heaps.size() - 1);
        }
        choices = solve(edges, n, root);
    }
    auto solve(auto edges, int n, int root) -> std::optional<std::vector<int>> {
        std::stack<std::tuple<int, int, std::vector<std::pair<int, int>>>> rings;
        std::vector<int> choice(n + 1);
        std::fill(parent.begin(), parent.end(), -1);
        mark[root] = root;
        for (int i = 1; i <= n; ++i) {
            if (i == root || mark[i]) { continue; }
            auto u = i;
            mark[u] = i;
            while (true) {
                while (h[u] && find(heaps[h[u]].from) == u) { h[u] = pop(h[u]); }
                if (!h[u]) { return std::nullopt; }
                choice[u] = heaps[h[u]].id;
                if (auto v = find(heaps[h[u]].from); mark[v] == i) {
                    std::vector<std::pair<int, int>> ring;
                    v = u;
                    auto stamp = stk.size();
                    do {
                        auto w = heaps[h[v]].from;
                        ring.emplace_back(v, heaps[h[v]].id);
                        update(h[v], -heaps[h[v]].val);
                        h[v] = pop(h[v]);
                        u = unite(u, v);
                        v = find(w);
                    } while (v != u);
                    rings.emplace(u, stamp, std::move(ring));
                } else if (!mark[v]) {
                    u = v;
                    mark[u] = i;
                } else {
                    break;
                }
            }
        }
        for (; !rings.empty(); rings.pop()) {
            auto &[u, stamp, ring] = rings.top();
            auto e = choice[u];
            rollback(stamp);
            for (const auto &[v, id]: ring) { choice[v] = id; }
            choice[find(std::get<1>(edges[e]))] = e;
        }
        return choice;
    }
};
