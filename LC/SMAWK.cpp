vi reduce(vi rows, vi cols) {// make #cols = #rows
    vi stk;                  // cols not indeices
    for (int i = 0; i < cols.size(); i++) {
        while (true) {
            if (stk.empty()) {
                stk.PB(cols[i]);
                break;
            } else {
                if (ask(rows[stk.size() - 1], stk.back()) <= ask(rows[stk.size() - 1], cols[i])) {
                    if (stk.size() + 1 <= rows.size()) {
                        stk.PB(cols[i]);
                        break;
                    } else {
                        break;
                    }
                } else {
                    stk.pop_back();
                }
            }
        }
    }
    return stk;
}
vi SMAWK(vi rows, vi cols) {
    if (rows.empty()) return vi();
    if (rows.size() < cols.size()) cols = reduce(rows, cols);
    vi rec_rows;
    for (int i = 1; i < rows.size(); i += 2) rec_rows.PB(rows[i]);
    vi rec_cols = SMAWK(rec_rows, cols);
    vi ans_cols;
    int last_idx = 0;
    for (int i = 0; i < rows.size(); i++) {
        if (i & 1) ans_cols.PB(rec_cols[i / 2]);
        else {
            int nxt_col = cols.back();
            if (i + 1 < rows.size()) nxt_col = rec_cols[(i + 1) / 2];
            int mn = ask(rows[i], cols[last_idx]), mn_col = cols[last_idx];
            while (last_idx + 1 < cols.size() && cols[last_idx + 1] <= nxt_col) {
                last_idx++;
                int nw = ask(rows[i], cols[last_idx]);
                if (nw < mn) mn = nw, mn_col = cols[last_idx];
            }
            ans_cols.PB(mn_col);
        }
    }
    return ans_cols;
}
