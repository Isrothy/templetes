#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numbers>
template<typename T> struct FibNode {
    T key;
    int degree;
    int size;
    bool mark;
    FibNode *parent, *child, *left, *right;
    explicit FibNode(T key) : key(std::move(key)), degree(0), size(1), mark(false), parent(nullptr), child(nullptr), left(this), right(this) {}
    void link_left(FibNode *other) {
        other->left->right = this;
        this->left->right = other;
        std::swap(this->left, other->left);
    }
};
template<typename T> void FibLink(FibNode<T> *x, FibNode<T> *y) {
    x->left->right = x->right;
    x->right->left = x->left;
    x->parent = y;
    if (!y->child) {
        y->child = x;
        x->left = x->right = x;
    } else {
        x->link_left(y->child);
    }
    y->degree++;
    x->mark = false;
}
template<typename T> FibNode<T> *consolidate(FibNode<T> *min, int size) {
    std::vector<FibNode<T> *> a(log(size) / log(std::numbers::phi) + 1, nullptr);
    auto w = min;
    do {
        auto x = w, next = x->right;
        int d = x->degree;
        x->left = x->right = x;
        while (a[d]) {
            auto y = a[d];
            if (x->key > y->key) { std::swap(x, y); }
            FibLink(y, x);
            a[d] = nullptr;
            ++d;
        }
        a[d] = x;
        w = next;
    } while (w != min);
    min = nullptr;
    for (auto &x: a) {
        if (x) {
            if (!min) {
                min = x;
            } else {
                min->link_left(x);
                if (x->key < min->key) { min = x; }
            }
        }
    }
    return min;
}
template<typename T> FibNode<T> *merge(FibNode<T> *x, FibNode<T> *y) {
    if (!x) { return y; }
    if (!y) { return x; }
    if (x->key > y->key) { std::swap(x, y); }
    x->link_left(y);
    x->size += y->size;
    return x;
}
template<typename T> FibNode<T> *insert(FibNode<T> *min, T key) {
    auto *node = new FibNode<T>(std::move(key));
    return merge(min, node);
}
template<typename T> std::pair<T, FibNode<T> *> extract_min(FibNode<T> *min) {
    T result = min->key;
    int size = min->size;
    auto child = min->child;
    if (child) {
        child->parent = nullptr;
        for (auto w = child->right; w != child; w = w->right) { w->parent = nullptr; }
        min->link_left(child);
    }
    if (min == min->right) {
        return std::make_pair(result, nullptr);
    } else {
        min->left->right = min->right;
        min->right->left = min->left;
        min = consolidate(min->right, size);
        min->size = size - 1;
    }
    return std::make_pair(result, min);
}
template<typename T> FibNode<T> *cut(FibNode<T> *min, FibNode<T> *x) {
    if (x->right == x) {
        x->parent->child = nullptr;
    } else {
        x->right->left = x->left;
        x->left->right = x->right;
        x->parent->child = x->right;
    }
    x->parent->degree--;
    x->parent = nullptr;
    x->left = x->right = x;
    x->mark = false;
    min->link_left(x);
    return min;
}
template<typename T> FibNode<T> *decrease_key(FibNode<T> *min, FibNode<T> *x, T key) {
    x->key = std::move(key);
    int size = min->size;
    auto y = x->parent;
    if (y && x->key < y->key) {
        min = cut(min, x);
        while (y->parent) {
            if (!y->mark) {
                y->mark = true;
                break;
            }
            auto z = y->parent;
            min = cut(min, y);
            y = z;
        }
    }
    if (x->key < min->key) { min = x; }
    min->size = size;
    return min;
}
