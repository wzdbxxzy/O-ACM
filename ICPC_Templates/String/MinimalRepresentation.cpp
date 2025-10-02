// Minimal Representation (最小表示法)
// 功能：求循环串的字典序最小表示
// 复杂度：O(n)
// 应用：循环同构、旋转字符串

#include <bits/stdc++.h>
using namespace std;

// 最小表示法（返回起始位置）
int minimal_representation(const string& s) {
    int n = s.size();
    int i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n) {
        int t = s[(i + k) % n] - s[(j + k) % n];

        if (t == 0) {
            k++;
        } else {
            if (t > 0) {
                i += k + 1;
            } else {
                j += k + 1;
            }

            if (i == j) j++;
            k = 0;
        }
    }

    return min(i, j);
}

// 最大表示法（返回起始位置）
int maximal_representation(const string& s) {
    int n = s.size();
    int i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n) {
        int t = s[(i + k) % n] - s[(j + k) % n];

        if (t == 0) {
            k++;
        } else {
            if (t < 0) {  // 注意这里与最小表示法相反
                i += k + 1;
            } else {
                j += k + 1;
            }

            if (i == j) j++;
            k = 0;
        }
    }

    return min(i, j);
}

// 获取最小表示串
string get_minimal_string(const string& s) {
    int pos = minimal_representation(s);
    int n = s.size();
    string result;
    for (int i = 0; i < n; i++) {
        result += s[(pos + i) % n];
    }
    return result;
}

// 获取最大表示串
string get_maximal_string(const string& s) {
    int pos = maximal_representation(s);
    int n = s.size();
    string result;
    for (int i = 0; i < n; i++) {
        result += s[(pos + i) % n];
    }
    return result;
}

// 判断两个串是否循环同构
bool is_cyclic_isomorphic(const string& s1, const string& s2) {
    if (s1.size() != s2.size()) return false;

    int pos1 = minimal_representation(s1);
    int pos2 = minimal_representation(s2);

    int n = s1.size();
    for (int i = 0; i < n; i++) {
        if (s1[(pos1 + i) % n] != s2[(pos2 + i) % n]) {
            return false;
        }
    }
    return true;
}

// 数组版本（对于整数序列）
template <typename T>
int minimal_representation_array(const vector<T>& a) {
    int n = a.size();
    int i = 0, j = 1, k = 0;

    while (i < n && j < n && k < n) {
        T diff = a[(i + k) % n] - a[(j + k) % n];

        if (diff == 0) {
            k++;
        } else {
            if (diff > 0) {
                i += k + 1;
            } else {
                j += k + 1;
            }

            if (i == j) j++;
            k = 0;
        }
    }

    return min(i, j);
}

// 使用示例
void solve() {
    string s;
    cin >> s;

    // 方法1：获取最小表示的起始位置
    int pos = minimal_representation(s);
    cout << "Minimal representation starts at: " << pos << "\n";

    // 方法2：直接获取最小表示串
    string min_str = get_minimal_string(s);
    cout << "Minimal string: " << min_str << "\n";

    // 方法3：获取最大表示串
    string max_str = get_maximal_string(s);
    cout << "Maximal string: " << max_str << "\n";
}

// 应用：项链问题
void necklace_problem() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    int pos = minimal_representation_array(a);

    cout << "Minimal representation: ";
    for (int i = 0; i < n; i++) {
        cout << a[(pos + i) % n] << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    // necklace_problem();

    return 0;
}
