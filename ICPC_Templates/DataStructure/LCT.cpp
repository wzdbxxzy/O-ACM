/**
 * Link-Cut Tree (LCT)
 * 功能：动态树，支持链接、切断、路径查询等
 * 复杂度：O(log n) 均摊
 */
#include <bits/stdc++.h>
using namespace std;

struct Node {
    Node *ch[2], *p;
    bool rev;
    int siz = 1;
    int val = 0, sum = 0;

    Node() : ch{nullptr, nullptr}, p(nullptr), rev(false) {}
};

void reverse(Node* t) {
    if (t) {
        swap(t->ch[0], t->ch[1]);
        t->rev ^= 1;
    }
}

void push(Node* t) {
    if (t->rev) {
        reverse(t->ch[0]);
        reverse(t->ch[1]);
        t->rev = false;
    }
}

void pull(Node* t) {
    t->siz =
        (t->ch[0] ? t->ch[0]->siz : 0) + 1 + (t->ch[1] ? t->ch[1]->siz : 0);
    t->sum = (t->ch[0] ? t->ch[0]->sum : 0) + t->val +
             (t->ch[1] ? t->ch[1]->sum : 0);
}

bool isroot(Node* t) {
    return t->p == nullptr || (t->p->ch[0] != t && t->p->ch[1] != t);
}

int pos(Node* t) { return t->p->ch[1] == t; }

void pushAll(Node* t) {
    if (!isroot(t)) {
        pushAll(t->p);
    }
    push(t);
}

void rotate(Node* t) {
    Node* q = t->p;
    int x = !pos(t);
    q->ch[!x] = t->ch[x];
    if (t->ch[x]) {
        t->ch[x]->p = q;
    }
    t->p = q->p;
    if (!isroot(q)) {
        q->p->ch[pos(q)] = t;
    }
    t->ch[x] = q;
    q->p = t;
    pull(q);
}

void splay(Node* t) {
    pushAll(t);
    while (!isroot(t)) {
        if (!isroot(t->p)) {
            if (pos(t) == pos(t->p)) {
                rotate(t->p);
            } else {
                rotate(t);
            }
        }
        rotate(t);
    }
    pull(t);
}

void access(Node* t) {
    for (Node *i = t, *q = nullptr; i; q = i, i = i->p) {
        splay(i);
        i->ch[1] = q;
        pull(i);
    }
    splay(t);
}

void makeroot(Node* t) {
    access(t);
    reverse(t);
}

void link(Node* x, Node* y) {
    makeroot(x);
    x->p = y;
}

void split(Node* x, Node* y) {
    makeroot(x);
    access(y);
}

void cut(Node* x, Node* y) {
    split(x, y);
    x->p = y->ch[0] = nullptr;
    pull(y);
}

int dist(Node* x, Node* y) {
    split(x, y);
    return y->siz - 1;
}

bool connected(Node* x, Node* y) {
    makeroot(x);
    access(y);
    while (x->p) x = x->p;
    return x == y;
}
