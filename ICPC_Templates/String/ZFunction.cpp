/**
 * Z函数 (Z-Algorithm)
 * 功能：计算字符串每个后缀与整个串的最长公共前缀
 * 复杂度：O(n)
 */
#include <bits/stdc++.h>
using namespace std;

vector<int> z_function(string s) {
    int n = s.size();
    vector<int> z(n + 1);
    z[0] = n;
    for (int i = 1, j = 1; i < n; i++) {
        z[i] = max(0, min(j + z[j] - i, z[i - j]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > j + z[j]) {
            j = i;
        }
    }
    return z;
}

// 字符串匹配：在文本t中查找模式串s的所有出现位置
vector<int> z_match(string s, string t) {
    string combined = s + "#" + t;
    vector<int> z = z_function(combined);
    vector<int> res;
    int n = s.size();
    for (int i = n + 1; i < combined.size(); i++) {
        if (z[i] == n) {
            res.push_back(i - n - 1);
        }
    }
    return res;
}
