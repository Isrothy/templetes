struct KdTree {
    static constexpr double alpha = 0.7;
    int x, y, val, sum, sz;
    int x_max, x_min, y_max, y_min;
    KdTree *ch[2];
    KdTree() = default;
    KdTree(int x, int y, int val) : x(x), y(y), val(val), sum(val), sz(1), x_max(x), x_min(x), y_max(y), y_min(y), ch{} {}
    void push_up() {
        sz = 1;
        sum = val;
        x_min = x_max = x;
        y_min = y_max = y;
        for (auto &c: ch) {
            if (c != nullptr) {
                sz += c->sz;
                sum += c->sum;
                x_min = std::min(x_min, c->x_min);
                x_max = std::max(x_max, c->x_max);
                y_min = std::min(y_min, c->y_min);
                y_max = std::max(y_max, c->y_max);
            }
        }
    }
};
KdTree *build(KdTree **p, int l, int r, int D = 1) {
    if (r < l) { return nullptr; }
    int mid = (l + r) >> 1;
    std::nth_element(p + l, p + mid, p + r + 1, [=](KdTree *p, KdTree *q) {
        if (D == 1) {
            return p->x == q->x ? p->y < q->y : p->x < q->x;
        } else {
            return p->y == q->y ? p->x < q->x : p->y < q->y;
        }
    });
    KdTree *root = p[mid];
    root->ch[0] = build(p, l, mid - 1, D ^ 1);
    root->ch[1] = build(p, mid + 1, r, D ^ 1);
    root->push_up();
    return root;
}
int query(KdTree *p, int x1, int y1, int x2, int y2) {
    if (p == nullptr) { return 0; }
    if (x2 < p->x_min || x1 > p->x_max || y2 < p->y_min || y1 > p->y_max) { return 0; }
    if (x1 <= p->x_min && p->x_max <= x2 && y1 <= p->y_min && p->y_max <= y2) { return p->sum; }
    int s = 0;
    if (x1 <= p->x && p->x <= x2 && y1 <= p->y && p->y <= y2) { s += p->val; }
    return s + query(p->ch[0], x1, y1, x2, y2) + query(p->ch[1], x1, y1, x2, y2);
}
KdTree *rebuild(KdTree *p, int D) {
    auto Q = new KdTree *[p->sz];
    int head = 0, tail = 0;
    Q[tail++] = p;
    while (head < tail) {
        KdTree *q = Q[head++];
        if (q->ch[0] != nullptr) { Q[tail++] = q->ch[0]; }
        if (q->ch[1] != nullptr) { Q[tail++] = q->ch[1]; }
    }
    auto ret = build(Q, 0, tail - 1, D);
    delete[] Q;
    return ret;
}
void insert(KdTree *&p, int x, int y, int a, int D = 1) {
    if (p == nullptr) {
        p = new KdTree(x, y, a);
        return;
    }
    if (p->x == x && p->y == y) {
        p->val += a;
        p->sum += a;
        return;
    }
    bool f;
    if (D == 1) {
        f = x == p->x ? p->y < y : p->x < x;
    } else {
        f = y == p->y ? p->x < x : p->y < y;
    }
    insert(p->ch[f], x, y, a, D ^ 1);
    p->push_up();
    if (p->sz * alpha < p->ch[f]->sz) { p = rebuild(p, D); }
}
