#include <cstring>
template<size_t M> struct KM {
    static const int INF = 0x3f3f3f3f;
    int d[M][M], Q[M], S[M], T[M], lx[M], ly[M], slack[M], pre[M];
    bool visx[M], visy[M];
    void add_edge(int u, int v, int c) {
        d[u][v] = max(d[u][v], c);
        lx[u] = max(lx[u], c);
    }
    int BFS(int u, int n) {
        memset(slack, 0x3f, sizeof slack);
        memset(pre, 0, sizeof pre);
        memset(visx, 0, sizeof visx);
        memset(visy, 0, sizeof visy);
        int head = 0, tail = 0;
        Q[tail++] = u;
        visx[u] = true;
        for (;;) {
            while (head < tail) {
                int x = Q[head++];
                for (int v = 1; v <= n; ++v) {
                    int gap = lx[x] + ly[v] - d[x][v];
                    if (slack[v] < gap or visy[v]) { continue; }
                    pre[v] = x;
                    if (gap == 0) {
                        if (T[v] == 0) { return v; }
                        visy[v] = visx[T[v]] = true;
                        Q[tail++] = T[v];
                    } else {
                        slack[v] = gap;
                    }
                }
            }
            int gap = INF;
            for (int v = 1; v <= n; ++v) {
                if (visy[v] == 0) { gap = min(gap, slack[v]); }
            }
            for (int v = 1; v <= n; ++v) {
                if (visx[v]) { lx[v] -= gap; }
                if (visy[v]) {
                    ly[v] += gap;
                } else {
                    slack[v] -= gap;
                }
            }
            for (int v = 1; v <= n; ++v) {
                if (visy[v] || slack[v]) { continue; }
                if (T[v] == 0) { return v; }
                visy[v] = visx[T[v]] = true;
                Q[tail++] = T[v];
            }
        }
    }
    long long maximum_weight_matching(int n) {
        for (int i = 1; i <= n; ++i) {
            if (S[i]) { continue; }
            int u = BFS(i, n);
            while (u) {
                T[u] = pre[u];
                swap(u, S[T[u]]);
            }
        }
        long long res = 0;
        for (int i = 1; i <= n; ++i) { res += d[i][S[i]]; }
        return res;
    }
};
