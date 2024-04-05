#include <cassert>
#include <iostream>
struct avl;
int safe_height(avl *p);
int safe_size(avl *p);
struct avl {
    int val, height;
    int size;
    avl *ch[2]{};
    explicit avl(int val) : val(val), height(1), size(1) {}
    avl *push_up() {
        size = 1 + safe_size(ch[0]) + safe_size(ch[1]);
        height = 1 + std::max(safe_height(ch[0]), safe_height(ch[1]));
        return this;
    }
    avl *rotate(int f) {
        avl *q = ch[f];
        ch[f] = q->ch[!f];
        q->ch[!f] = this;
        push_up();
        q->push_up();
        return q;
    }
    avl *maintain(int f) {
        if (safe_height(ch[f]) - safe_height(ch[!f]) == 2) {
            if (safe_height(ch[f]->ch[f]) < safe_height(ch[f]->ch[!f])) { ch[f] = ch[f]->rotate(!f); }
            return rotate(f);
        }
        return this;
    }
};
int safe_size(avl *p) { return p == nullptr ? 0 : p->size; }
int safe_height(avl *p) { return p == nullptr ? 0 : p->height; }
avl *insert(avl *p, int x) {
    if (p == nullptr) { return new avl(x); }
    bool f = p->val < x;
    p->ch[f] = insert(p->ch[f], x);
    return p->push_up()->maintain(f);
}
avl *erase(avl *p, int x) {
    if (p->val == x) {
        if (p->ch[0] == nullptr || p->ch[1] == nullptr) {
            avl *ret = p->ch[p->ch[0] == nullptr];
            delete p;
            return ret;
        }
        avl *q = p->ch[1];
        while (q->ch[0] != nullptr) { q = q->ch[0]; }
        p->val = q->val;
        p->ch[1] = erase(p->ch[1], q->val);
        return p->push_up()->maintain(0);
    } else {
        bool f = p->val < x;
        p->ch[f] = erase(p->ch[f], x);
        return p->push_up()->maintain(!f);
    }
}
