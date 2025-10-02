// Dancing Links X (DLX / 舞蹈链)
// 功能：高效解决精确覆盖问题
// 复杂度：指数级，但剪枝效果很好
// 应用：数独、N皇后、精确覆盖、重复覆盖

#include <bits/stdc++.h>
using namespace std;

struct DLX {
    static const int MAXN = 1e5 + 10;
    static const int MAXM = 1e3 + 10;

    int n, m, sz;              // n行m列，sz为节点总数
    int S[MAXM];               // S[i]表示第i列有多少个1
    int row[MAXN], col[MAXN];  // row[i], col[i]表示节点i所在的行列
    int L[MAXN], R[MAXN], U[MAXN], D[MAXN];  // 四个方向的指针
    int ansd, ans[MAXN];                     // 答案深度和答案数组

    // 初始化
    void init(int _n, int _m) {
        n = _n;
        m = _m;

        // 初始化列头节点（0号节点为表头）
        for (int i = 0; i <= m; i++) {
            S[i] = 0;
            U[i] = D[i] = i;
            L[i] = i - 1;
            R[i] = i + 1;
        }
        L[0] = m;
        R[m] = 0;
        sz = m + 1;
    }

    // 在第r行第c列插入一个节点
    void link(int r, int c) {
        row[sz] = r;
        col[sz] = c;
        S[c]++;

        // 插入到第c列
        D[sz] = D[c];
        U[D[c]] = sz;
        U[sz] = c;
        D[c] = sz;

        // 插入到第r行
        if (!row[sz - 1] || row[sz - 1] != r) {
            L[sz] = R[sz] = sz;
        } else {
            R[sz] = R[sz - 1];
            L[R[sz - 1]] = sz;
            L[sz] = sz - 1;
            R[sz - 1] = sz;
        }

        sz++;
    }

    // 移除第c列
    void remove(int c) {
        L[R[c]] = L[c];
        R[L[c]] = R[c];

        for (int i = D[c]; i != c; i = D[i]) {
            for (int j = R[i]; j != i; j = R[j]) {
                U[D[j]] = U[j];
                D[U[j]] = D[j];
                S[col[j]]--;
            }
        }
    }

    // 恢复第c列
    void restore(int c) {
        for (int i = U[c]; i != c; i = U[i]) {
            for (int j = L[i]; j != i; j = L[j]) {
                U[D[j]] = j;
                D[U[j]] = j;
                S[col[j]]++;
            }
        }

        L[R[c]] = c;
        R[L[c]] = c;
    }

    // 精确覆盖求解
    bool dance(int d) {
        if (R[0] == 0) {
            ansd = d;
            return true;
        }

        // 选择S[c]最小的列（启发式剪枝）
        int c = R[0];
        for (int i = R[0]; i != 0; i = R[i]) {
            if (S[i] < S[c]) {
                c = i;
            }
        }

        remove(c);

        for (int i = D[c]; i != c; i = D[i]) {
            ans[d] = row[i];

            for (int j = R[i]; j != i; j = R[j]) {
                remove(col[j]);
            }

            if (dance(d + 1)) {
                return true;
            }

            for (int j = L[i]; j != i; j = L[j]) {
                restore(col[j]);
            }
        }

        restore(c);
        return false;
    }

    // 求所有解的数量
    int count_solutions(int d) {
        if (R[0] == 0) {
            return 1;
        }

        int c = R[0];
        for (int i = R[0]; i != 0; i = R[i]) {
            if (S[i] < S[c]) {
                c = i;
            }
        }

        if (S[c] == 0) return 0;  // 无解

        remove(c);
        int res = 0;

        for (int i = D[c]; i != c; i = D[i]) {
            for (int j = R[i]; j != i; j = R[j]) {
                remove(col[j]);
            }

            res += count_solutions(d + 1);

            for (int j = L[i]; j != i; j = L[j]) {
                restore(col[j]);
            }
        }

        restore(c);
        return res;
    }
};

// 应用1：数独求解
void solve_sudoku() {
    DLX dlx;

    // 数独有4种约束：
    // 1. 每个格子恰好填一个数（9*9=81列）
    // 2. 每行每个数字恰好出现一次（9*9=81列）
    // 3. 每列每个数字恰好出现一次（9*9=81列）
    // 4. 每个3x3宫格每个数字恰好出现一次（9*9=81列）
    // 共324列

    char board[9][9];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cin >> board[i][j];
        }
    }

    dlx.init(9 * 9 * 9, 324);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                if (board[i][j] != '.' && board[i][j] != '0' + k + 1) {
                    continue;
                }

                int row_id = i * 81 + j * 9 + k + 1;

                // 约束1：格子(i,j)
                dlx.link(row_id, i * 9 + j + 1);

                // 约束2：第i行数字k+1
                dlx.link(row_id, 81 + i * 9 + k + 1);

                // 约束3：第j列数字k+1
                dlx.link(row_id, 162 + j * 9 + k + 1);

                // 约束4：第(i/3, j/3)宫格数字k+1
                int block = (i / 3) * 3 + (j / 3);
                dlx.link(row_id, 243 + block * 9 + k + 1);
            }
        }
    }

    if (dlx.dance(0)) {
        cout << "Solution found!\n";
        for (int i = 0; i < dlx.ansd; i++) {
            int id = dlx.ans[i] - 1;
            int row = id / 81;
            int col = (id % 81) / 9;
            int num = id % 9 + 1;
            board[row][col] = '0' + num;
        }

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cout << board[i][j];
            }
            cout << "\n";
        }
    } else {
        cout << "No solution\n";
    }
}

// 应用2：N皇后问题
void solve_n_queens(int n) {
    DLX dlx;

    // 约束：
    // 1. 每行恰好一个皇后（n列）
    // 2. 每列恰好一个皇后（n列）
    // 3. 每条主对角线最多一个皇后（2n-1列）
    // 4. 每条副对角线最多一个皇后（2n-1列）

    int total_cols = 2 * n + 2 * (2 * n - 1);
    dlx.init(n * n, total_cols);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int row_id = i * n + j + 1;

            // 约束1：第i行
            dlx.link(row_id, i + 1);

            // 约束2：第j列
            dlx.link(row_id, n + j + 1);

            // 约束3：主对角线i-j+n-1
            dlx.link(row_id, 2 * n + (i - j + n - 1) + 1);

            // 约束4：副对角线i+j
            dlx.link(row_id, 2 * n + (2 * n - 1) + (i + j) + 1);
        }
    }

    int solutions = dlx.count_solutions(0);
    cout << n << "-Queens: " << solutions << " solutions\n";
}

// 应用3：精确覆盖问题（通用）
void exact_cover() {
    int n, m;  // n行m列
    cin >> n >> m;

    DLX dlx;
    dlx.init(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int val;
            cin >> val;
            if (val == 1) {
                dlx.link(i + 1, j + 1);
            }
        }
    }

    if (dlx.dance(0)) {
        cout << "Solution:\n";
        for (int i = 0; i < dlx.ansd; i++) {
            cout << "Row " << dlx.ans[i] << "\n";
        }
    } else {
        cout << "No solution\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // solve_sudoku();
    solve_n_queens(8);
    // exact_cover();

    return 0;
}
