/**
 * KMP算法 (Knuth-Morris-Pratt)
 * 功能：字符串匹配、计算前缀函数
 * 复杂度：O(n + m)
 */
#include <bits/stdc++.h>
using namespace std;

// 计算前缀函数（失配函数）
vector<int> kmp(string s) {
    int n = s.size();
    vector<int> f(n + 1);
    for (int i = 1, j = 0; i < n; i++) {
        while (j && s[i] != s[j]) {
            j = f[j];
        }
        j += (s[i] == s[j]);
        f[i + 1] = j;
    }
    return f;
}

// 字符串匹配：在文本t中查找模式串s的所有出现位置
vector<int> kmp_match(string s, string t) {
    int n = s.size(), m = t.size();
    vector<int> f = kmp(s);
    vector<int> res;

    for (int i = 0, j = 0; i < m; i++) {
        while (j && t[i] != s[j]) {
            j = f[j];
        }
        j += (t[i] == s[j]);
        if (j == n) {
            res.push_back(i - n + 1);
            j = f[j];
        }
    }
    return res;
}
