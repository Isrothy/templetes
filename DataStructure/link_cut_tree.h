struct Lct {
    unsigned val, sum;
    bool rev;
    Lct *ch[2], *fa;
    Lct() = default;
    explicit Lct(unsigned val) : val(val), sum(val), rev(false), ch{nullptr, nullptr}, fa(nullptr) {}
    Lct *push_up() {
        sum = val;
        if (ch[0]) { sum ^= ch[0]->sum; }
        if (ch[1]) { sum ^= ch[1]->sum; }
        return this;
    }
    bool dir() const { return fa->ch[1] == this; }
    bool is_root() const { return !fa || (fa->ch[0] != this && fa->ch[1] != this); }
    Lct *update_rev() {
        rev ^= 1;
        std::swap(ch[0], ch[1]);
        return this;
    }
    Lct *push_down() {
        if (rev) {
            if (ch[0]) { ch[0]->update_rev(); }
            if (ch[1]) { ch[1]->update_rev(); }
            rev = false;
        }
        return this;
    }
    Lct *rotate(bool f) {
        auto q = ch[f];
        ch[f] = q->ch[!f];
        if (ch[f]) { ch[f]->fa = this; }
        q->fa = fa;
        if (!is_root()) { fa->ch[dir()] = q; }
        q->ch[!f] = this;
        fa = q;
        push_up();
        return q;
    }
    Lct *splay() {
        std::stack<Lct *> stk;
        for (auto q = this; !q->is_root(); q = q->fa) { stk.push(q->fa); }
        while (!stk.empty()) {
            stk.top()->push_down();
            stk.pop();
        }
        push_down();
        while (!is_root()) {
            bool f = dir();
            if (!fa->is_root()) { (fa->dir() == f ? fa->fa : fa)->rotate(f); }
            fa->rotate(dir());
        }
        return push_up();
    }
    auto access() {
        Lct *p = nullptr;
        for (Lct *q = this; q; q = q->fa) {
            q->splay();
            q->ch[1] = p;
            q->push_up();
            p = q;
        }
        return this;
    }
    auto make_root() { return access()->splay()->update_rev(); }
    auto find_root() {
        access()->splay();
        auto q = this;
        while (q->ch[0]) { q = q->push_down()->ch[0]; }
        return q->splay();
    }
};
void link(Lct *p, Lct *q) {
    if (p->find_root() != q->find_root()) { p->make_root()->fa = q; }
}
void cut(Lct *p, Lct *q) {
    p->make_root();
    q->access()->splay();
    auto r = q->ch[0];
    if (!r) { return; }
    while (r->ch[1]) { r = r->push_down()->ch[1]; }
    if (r == p) {
        q->ch[0]->fa = nullptr;
        q->ch[0] = nullptr;
    }
    r->splay();
}
std::optional<unsigned> query(Lct *p, Lct *q) {
    if (p->find_root() != q->find_root()) { return std::nullopt; }
    p->make_root();
    q->access()->splay();
    return q->sum;
}
void update(Lct *p, unsigned val) {
    p->make_root();
    p->val = val;
    p->push_up();
}
