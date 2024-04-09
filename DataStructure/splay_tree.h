struct splay_node;
int safe_size(splay_node *p);
struct splay_node {
    int val, size;
    bool rev;
    splay_node *ch[2], *fa;
    explicit splay_node(int val) : val(val), size(1), rev(false), ch{nullptr, nullptr}, fa(nullptr) {}
    bool dir() { return fa->ch[1] == this; }
    splay_node *update_rev() {
        rev ^= 1;
        std::swap(ch[0], ch[1]);
        return this;
    }
    splay_node *push_up() {
        size = 1 + safe_size(ch[0]) + safe_size(ch[1]);
        return this;
    }
    splay_node *push_down() {
        if (rev) {
            if (ch[0] != nullptr) { ch[0]->update_rev(); }
            if (ch[1] != nullptr) { ch[1]->update_rev(); }
            rev = false;
        }
        return this;
    }
    splay_node *rotate(bool f) {
        splay_node *q = ch[f];
        ch[f] = q->ch[!f];
        if (ch[f] != nullptr) { ch[f]->fa = this; }
        q->fa = fa;
        if (fa != nullptr) { fa->ch[dir()] = q; }
        q->ch[!f] = this;
        fa = q;
        push_up();
        return q;
    }
    splay_node *splay_to(splay_node *t) {
        std::stack<splay_node *> stk;
        for (splay_node *q = this; q->fa != t; q = q->fa) { stk.push(q->fa); }
        while (!stk.empty()) {
            stk.top()->push_down();
            stk.pop();
        }
        push_down();
        while (fa != t) {
            bool f = dir();
            if (fa->fa != t) { (fa->dir() == f ? fa->fa : fa)->rotate(f); }
            fa->rotate(dir());
        }
        return push_up();
    }
};
int safe_size(splay_node *p) { return p == nullptr ? 0 : p->size; }
