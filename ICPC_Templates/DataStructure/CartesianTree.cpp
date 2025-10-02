/**
 * 笛卡尔树 (Cartesian Tree)
 * 功能：RMQ、最大矩形面积等
 * 复杂度：O(n) 构建
 */
#include <bits/stdc++.h>
using namespace std;

// 构建笛卡尔树（按小根堆性质）
vector<int> buildCartesianTree(const vector<int>& a) {
    int n = a.size();
    vector<int> parent(n, -1);
    vector<int> left(n, -1);
    vector<int> right(n, -1);

    stack<int> stk;

    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!stk.empty() && a[stk.top()] > a[i]) {
            last = stk.top();
            stk.pop();
        }

        if (!stk.empty()) {
            right[stk.top()] = i;
            parent[i] = stk.top();
        }

        if (last != -1) {
            left[i] = last;
            parent[last] = i;
        }

        stk.push(i);
    }

    return parent;
}

// 使用笛卡尔树求最大矩形面积
long long maxRectangleArea(const vector<int>& heights) {
    int n = heights.size();
    vector<int> left(n), right(n);
    stack<int> stk;

    // 找每个位置左边第一个小于它的位置
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.top()] >= heights[i]) {
            stk.pop();
        }
        left[i] = stk.empty() ? -1 : stk.top();
        stk.push(i);
    }

    while (!stk.empty()) stk.pop();

    // 找每个位置右边第一个小于它的位置
    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && heights[stk.top()] >= heights[i]) {
            stk.pop();
        }
        right[i] = stk.empty() ? n : stk.top();
        stk.push(i);
    }

    long long maxArea = 0;
    for (int i = 0; i < n; i++) {
        long long width = right[i] - left[i] - 1;
        maxArea = max(maxArea, width * heights[i]);
    }

    return maxArea;
}
