struct DLX {
    struct node {
        int i, j;
        node *left, *right, *up, *down;
        node() : i(0), j(0), left(nullptr), right(nullptr), up(nullptr), down(nullptr) {}
        explicit node(int i = 0, int j = 0) {
            this->i = i;
            this->j = j;
            left = right = up = down = nullptr;
        }
    };
    std::vector<node *> row, col;
    std::vector<int> cnt;
    node *sentinel;
    DLX(int r, int c) {
        sentinel = new node(0, 0);
        col = std::vector<node *>(c + 1);
        row = std::vector<node *>(r + 1);
        cnt = std::vector<int>(c + 1);
        col[0] = row[0] = sentinel;
        for (int i = 1; i <= c; ++i) {
            col[i] = new node(0, i);
            col[i]->up = col[i]->down = col[i];
        }
        for (int i = 0; i <= c; ++i) {
            col[i]->left = col[(i + c) % (c + 1)];
            col[i]->right = col[(i + 1) % (c + 1)];
        }
        for (int j = 1; j <= r; ++j) {
            row[j] = new node(j, 0);
            row[j]->left = row[j]->right = row[j];
        }
        for (int j = 0; j <= r; ++j) {
            row[j]->up = row[(j + r) % (r + 1)];
            row[j]->down = row[(j + 1) % (r + 1)];
        }
    }
    void insert_back(int r, int c) {
        auto p = new node(r, c);
        p->up = col[c]->up;
        p->down = col[c];
        p->left = row[r]->left;
        p->right = row[r];
        p->up->down = p->down->up = p;
        p->left->right = p->right->left = p;
        ++cnt[c];
    }
    void remove(node *p) {
        p->left->right = p->right;
        p->right->left = p->left;
        for (auto q = p->down; q != p; q = q->down) {
            for (auto r = q->right; r != q; r = r->right) {
                r->up->down = r->down;
                r->down->up = r->up;
                --cnt[r->j];
            }
        }
    }
    void recover(node *p) {
        for (auto q = p->up; q != p; q = q->up) {
            for (auto r = q->left; r != q; r = r->left) {
                r->down->up = r;
                r->up->down = r;
                ++cnt[r->j];
            }
        }
        p->right->left = p;
        p->left->right = p;
    }
    bool dance(int depth, std::vector<int> &ans) {
        if (sentinel->right == sentinel) {
            ans.resize(depth);
            return true;
        }
        if (depth == ans.size()) { ans.push_back(0); }
        node *p = sentinel->right;
        for (auto q = sentinel->right; q != sentinel; q = q->right) {
            if (cnt[q->j] < cnt[p->j]) { p = q; }
        }
        remove(p);
        for (auto q = p->down; q != p; q = q->down) {
            ans[depth] = q->i;
            for (auto r = q->right; r != q; r = r->right) {
                if (r->j) { remove(col[r->j]); }
            }
            if (dance(depth + 1, ans)) { return true; }
            for (auto r = q->left; r != q; r = r->left) {
                if (r->j) { recover(col[r->j]); }
            }
        }
        recover(p);
        return false;
    }
    std::vector<int> solve() {
        std::vector<int> ans;
        if (dance(0, ans)) { return ans; }
        return {};
    }
};
