int stoer_wagner(int d[M][M], int n) {
    static int w[M];
    static bool vis[M], del[M];
    int res = INF;
    for (int u = 1; u <= n; ++u) { del[u] = false; }
    for (int i = 1; i < n; ++i) {
        for (int u = 1; u <= n; ++u) {
            w[u] = 0;
            vis[u] = false;
        }
        int s = -1, t = -1;
        for (int j = 1; j <= n - i + 1; ++j) {
            int v = -1;
            for (int u = 1; u <= n; ++u) {
                if (!del[u] && !vis[u] && (v == -1 || w[v] < w[u])) { v = u; }
            }
            vis[v] = true;
            for (int u = 1; u <= n; ++u) {
                if (!del[u] && !vis[u]) { w[u] += d[u][v]; }
            }
            s = t;
            t = v;
        }
        res = min(res, w[t]);
        del[t] = true;
        for (int u = 1; u <= n; ++u) {
            d[u][s] += d[u][t];
            d[s][u] += d[t][u];
        }
    }
    return res;
}
