/**
 * 可持久化线段树 (Persistent Segment Tree / Chairman Tree)
 * 功能：维护历史版本，区间k小值等
 * 复杂度：O(log n) 单次操作
 */
#include <bits/stdc++.h>
using namespace std;

struct Node {
    Node* l = nullptr;
    Node* r = nullptr;
    int cnt = 0;
};

Node* build(int l, int r) {
    Node* p = new Node();
    if (r - l == 1) {
        return p;
    }
    int m = (l + r) / 2;
    p->l = build(l, m);
    p->r = build(m, r);
    return p;
}

Node* add(Node* t, int l, int r, int p, int v) {
    Node* x = new Node();
    if (t) {
        *x = *t;
    }
    x->cnt += v;
    if (r - l == 1) {
        return x;
    }
    int m = (l + r) / 2;
    if (p < m) {
        x->l = add(x->l, l, m, p, v);
    } else {
        x->r = add(x->r, m, r, p, v);
    }
    return x;
}

// 区间第k小
int query(Node* tl, Node* tr, int l, int r, int k) {
    if (r - l == 1) {
        return l;
    }
    int m = (l + r) / 2;
    int cnt = (tr->l ? tr->l->cnt : 0) - (tl->l ? tl->l->cnt : 0);
    if (k <= cnt) {
        return query(tl->l, tr->l, l, m, k);
    } else {
        return query(tl->r, tr->r, m, r, k - cnt);
    }
}

// 区间值域查询
pair<int, int> rangeQuery(Node* tl, Node* tr, int l, int r, int x, int y) {
    if (l >= y || r <= x || !tl || !tr) {
        return {0, 0};
    }
    if (l >= x && r <= y) {
        return {tr->cnt - tl->cnt, tr->cnt - tl->cnt};
    }
    int m = (l + r) / 2;
    auto [cl, sl] = rangeQuery(tl->l, tr->l, l, m, x, y);
    auto [cr, sr] = rangeQuery(tl->r, tr->r, m, r, x, y);
    return {cl + cr, sl + sr};
}
