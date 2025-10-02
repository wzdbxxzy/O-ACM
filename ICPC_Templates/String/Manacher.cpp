/**
 * Manacher算法
 * 功能：求最长回文子串
 * 复杂度：O(n)
 */
#include <bits/stdc++.h>
using namespace std;

// 返回以每个位置为中心的最长回文半径
vector<int> manacher(string s) {
    int n = s.size();
    string t = "#";
    for (char c : s) {
        t += c;
        t += "#";
    }
    int m = t.size();
    vector<int> d(m);

    for (int i = 0, l = 0, r = -1; i < m; i++) {
        int k = (i > r) ? 1 : min(d[l + r - i], r - i + 1);
        while (0 <= i - k && i + k < m && t[i - k] == t[i + k]) {
            k++;
        }
        d[i] = k--;
        if (i + k > r) {
            l = i - k;
            r = i + k;
        }
    }
    return d;
}

// 求最长回文子串
pair<int, int> longest_palindrome(string s) {
    vector<int> d = manacher(s);
    int maxLen = 0, center = 0;
    for (int i = 0; i < d.size(); i++) {
        if (d[i] > maxLen) {
            maxLen = d[i];
            center = i;
        }
    }
    maxLen--;
    int start = (center - maxLen) / 2;
    return {start, maxLen};
}
