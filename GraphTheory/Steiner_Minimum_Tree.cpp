int Steiner_Minimum_Tree(int *s, int k) {
    for (int S = 0; S < 1 << k; ++S) {
        for (int u = 1; u <= n; ++u) { dp[S][u] = INF; }
    }
    for (int i = 0; i < k; ++i) { dp[1 << i][s[i]] = 0; }
    for (int S = 1; S < 1 << k; ++S) {
        for (int T = (S - 1) & S; T; --T &= S) {
            for (int u = 1; u <= n; ++u) { dp[S][u] = min(dp[S][u], dp[T][u] + dp[S ^ T][u]); }
        }
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
        for (int u = 1; u <= n; ++u) { Q.push(make_pair(dp[S][u], u)); }
        while (!Q.empty()) {
            pair<int, int> p = Q.top();
            Q.pop();
            int u = p.second;
            if (p.first != dp[S][u]) { continue; }
            for (auto e: E[u]) {
                int v = e.first;
                if (p.first + e.second < dp[S][v]) {
                    dp[S][v] = p.first + e.second;
                    Q.push(make_pair(dp[S][v], v));
                }
            }
        }
    }
    int res = INF;
    for (int u = 1; u <= n; ++u) { res = min(res, dp[(1 << k) - 1][u]); }
    return res;
}
