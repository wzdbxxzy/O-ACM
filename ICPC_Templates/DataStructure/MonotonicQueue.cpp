/**
 * 单调队列 (Monotonic Queue)
 * 功能：维护滑动窗口最值
 * 复杂度：O(n)
 */
#include <bits/stdc++.h>
using namespace std;

// 单调队列求滑动窗口最小值
template <typename T>
vector<T> slidingWindowMin(const vector<T>& a, int k) {
    int n = a.size();
    vector<T> res;
    deque<int> q;

    for (int i = 0; i < n; i++) {
        // 移除不在窗口内的元素
        while (!q.empty() && q.front() <= i - k) {
            q.pop_front();
        }
        // 维护单调性
        while (!q.empty() && a[q.back()] >= a[i]) {
            q.pop_back();
        }
        q.push_back(i);

        if (i >= k - 1) {
            res.push_back(a[q.front()]);
        }
    }
    return res;
}

// 单调队列求滑动窗口最大值
template <typename T>
vector<T> slidingWindowMax(const vector<T>& a, int k) {
    int n = a.size();
    vector<T> res;
    deque<int> q;

    for (int i = 0; i < n; i++) {
        while (!q.empty() && q.front() <= i - k) {
            q.pop_front();
        }
        while (!q.empty() && a[q.back()] <= a[i]) {
            q.pop_back();
        }
        q.push_back(i);

        if (i >= k - 1) {
            res.push_back(a[q.front()]);
        }
    }
    return res;
}

// 单调栈求每个元素左边第一个比它小的元素
template <typename T>
vector<int> prevSmaller(const vector<T>& a) {
    int n = a.size();
    vector<int> res(n, -1);
    stack<int> stk;

    for (int i = 0; i < n; i++) {
        while (!stk.empty() && a[stk.top()] >= a[i]) {
            stk.pop();
        }
        if (!stk.empty()) {
            res[i] = stk.top();
        }
        stk.push(i);
    }
    return res;
}

// 单调栈求每个元素右边第一个比它小的元素
template <typename T>
vector<int> nextSmaller(const vector<T>& a) {
    int n = a.size();
    vector<int> res(n, n);
    stack<int> stk;

    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && a[stk.top()] >= a[i]) {
            stk.pop();
        }
        if (!stk.empty()) {
            res[i] = stk.top();
        }
        stk.push(i);
    }
    return res;
}
