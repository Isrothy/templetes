#include <iostream>
#include <random>
struct treap;
int safe_size(treap *p);
std::mt19937_64 mt_rand(std::random_device{}());
struct treap {
    int val, size;
    unsigned long long priority;
    treap *ch[2]{};
    explicit treap(int val) : val(val), size(1), priority(mt_rand()) {}
    treap *push_up() {
        size = 1 + safe_size(ch[0]) + safe_size(ch[1]);
        return this;
    }
    treap *push_down() { return this; }
    treap *rotate(int f) {
        treap *q = ch[f];
        ch[f] = q->ch[!f];
        q->ch[!f] = this;
        push_up();
        q->push_up();
        return q;
    }
};
typedef std::pair<treap *, treap *> ptt;
int safe_size(treap *p) { return p == nullptr ? 0 : p->size; }
treap *insert(treap *p, int x) {
    if (p == nullptr) { return new treap(x); }
    bool f = p->val < x;
    p->ch[f] = insert(p->ch[f], x);
    if (p->ch[f]->priority < p->priority) { p = p->rotate(f); }
    return p->push_up();
}
treap *erase(treap *p, int x) {
    if (x == p->val) {
        if (p->ch[0] == nullptr || p->ch[1] == nullptr) { return p->ch[p->ch[0] == nullptr]; }
        bool f = p->ch[1]->priority < p->ch[0]->priority;
        p = p->rotate(f);
        p->ch[!f] = erase(p->ch[!f], x);
    } else {
        bool f = p->val < x;
        p->ch[f] = erase(p->ch[f], x);
    }
    return p->push_up();
}
int rank(treap *p, int x) {
    int res = 1;
    while (p != nullptr) {
        if (p->val < x) {
            res += safe_size(p->ch[0]) + 1;
            p = p->ch[1];
        } else {
            p = p->ch[0];
        }
    }
    return res;
}
int kth(treap *p, int k) {
    for (;;) {
        int s = safe_size(p->ch[0]);
        if (s + 1 == k) { return p->val; }
        if (k <= s) {
            p = p->ch[0];
        } else {
            k -= s + 1;
            p = p->ch[1];
        }
    }
}
int pre(treap *p, int x) {
    int res = -1;
    while (p != nullptr) {
        if (p->val < x) {
            res = p->val;
            p = p->ch[1];
        } else {
            p = p->ch[0];
        }
    }
    return res;
}
int suc(treap *p, int x) {
    int res = -1;
    while (p != nullptr) {
        if (x < p->val) {
            res = p->val;
            p = p->ch[0];
        } else {
            p = p->ch[1];
        }
    }
    return res;
}
treap *merge(treap *p, treap *q) {
    if (!p) { return q; }
    if (!q) { return p; }
    p->push_down();
    q->push_down();
    if (p->priority < q->priority) {
        p->ch[1] = merge(p->ch[1], q);
        return p->push_up();
    } else {
        q->ch[0] = merge(p, q->ch[0]);
        return q->push_up();
    }
}
ptt split(treap *p, int k) {
    if (p == nullptr) { return {nullptr, nullptr}; }
    p->push_down();
    if (k <= safe_size(p->ch[0])) {
        ptt o = split(p->ch[0], k);
        p->ch[0] = o.second;
        p->push_up();
        return {o.first, p};
    } else {
        ptt o = split(p->ch[1], k - safe_size(p->ch[0]) - 1);
        p->ch[1] = o.first;
        p->push_up();
        return {p, o.second};
    }
}
std::pair<treap *, treap *> split_by_value(treap *p, int v) {
    if (!p) { return {}; }
    if (v < p->val) {
        ptt o = split_by_value(p->ch[0], v);
        p->ch[0] = o.second;
        p->push_up();
        return {o.first, p};
    } else {
        ptt o = split_by_value(p->ch[1], v);
        p->ch[1] = o.first;
        p->push_up();
        return {p, o.second};
    }
}
treap *heuristic_merge(treap *p, treap *q) {
    if (!p) { return q; }
    if (!q) { return p; }
    if (p->priority < q->priority) { std::swap(p, q); }
    ptt o = split_by_value(p, q->val);
    q->ch[0] = heuristic_merge(q->ch[0], o.first);
    q->ch[1] = heuristic_merge(q->ch[1], o.second);
    q->push_up();
    return q;
}
