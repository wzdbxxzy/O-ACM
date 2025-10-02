# 快速参考手册

## 时间复杂度参考

### 常见复杂度与数据规模对应

- O(1): 任意
- O(log n): 10^18
- O(n): 10^8
- O(n log n): 10^6
- O(n√n): 10^5
- O(n²): 5000
- O(n³): 500
- O(2^n): 20
- O(n!): 11

## 常用技巧

### 1. 数据结构选择

**维护区间最值**

- 静态：ST 表 O(n log n) 预处理，O(1) 查询
- 动态：线段树 O(log n) 单次操作
- 滑动窗口：单调队列 O(n)

**维护区间和**

- 静态：前缀和 O(1)
- 动态单点修改：树状数组 O(log n)
- 动态区间修改：线段树 O(log n)

**维护集合**

- 查询存在性：set / unordered_set
- 计数：map / unordered_map
- 连通性：并查集

### 2. 图论算法选择

**最短路**

- 无权图：BFS O(n + m)
- 非负权：Dijkstra O((n + m) log n)
- 有负权：Bellman-Ford O(nm) 或 SPFA
- 多源最短路：Floyd O(n³)

**网络流**

- 最大流：Dinic O(n²m)
- 最小费用最大流：MCMF O(nmf)

**图的性质**

- 强连通分量：Tarjan O(n + m)
- 割点割边：Tarjan O(n + m)
- 2-SAT：转化为 SCC
- 最小生成树：Kruskal O(m log m)

### 3. 字符串算法

**模式匹配**

- 单模式串：KMP O(n + m)
- 多模式串：AC 自动机 O(n + m + Σ)
- 子串查找：字符串哈希 O(n)

**回文串**

- 最长回文：Manacher O(n)
- 回文树：PAM

**后缀相关**

- 后缀数组：O(n log n)
- 后缀自动机：O(n)

### 4. 数学技巧

**取模运算**

- 快速幂：O(log n)
- 逆元：扩展欧几里得 或 费马小定理
- 组合数：预处理阶乘和逆元

**素数**

- 判定单个：Miller-Rabin O(k log³ n)
- 筛法：欧拉筛 O(n)

**多项式**

- 卷积：NTT O(n log n)

### 5. 动态规划技巧

**常见 DP 类型**

- 线性 DP：背包、LIS、LCS
- 区间 DP：石子合并
- 树形 DP：树上背包
- 状压 DP：集合 DP
- 数位 DP：计数问题

**优化技巧**

- 滚动数组：空间优化
- 单调队列/栈：O(n) 优化转移
- 矩阵快速幂：优化递推
- 斜率优化：凸包维护

### 6. 计算几何

**基础运算**

- 点积：判断角度
- 叉积：判断方向、面积
- 判断点在多边形内：射线法

**凸包**

- Andrew 算法：O(n log n)
- 旋转卡壳：求直径

## 常见错误

1. **数组越界**

   - 开数组时多开一点
   - 注意边界条件

2. **整数溢出**

   - 乘法前先转 long long
   - 注意中间结果可能溢出

3. **除零错误**

   - 除法前检查除数
   - 取模运算注意模数为 0

4. **精度问题**

   - 浮点数比较用 eps
   - 能用整数就不用浮点

5. **特殊情况**
   - n = 0, n = 1
   - 空图、空串
   - 全部相同

## 调试技巧

```cpp
#define debug(x) cerr << #x << " = " << (x) << endl

// 输出数组
template<typename T>
void print(const vector<T> &v) {
    for (auto x : v) cerr << x << " ";
    cerr << endl;
}

// 计时
auto start = chrono::high_resolution_clock::now();
// ... 代码
auto end = chrono::high_resolution_clock::now();
cerr << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
```

## 输入输出技巧

```cpp
// 关闭同步加速
ios::sync_with_stdio(false);
cin.tie(nullptr);

// 读入直到EOF
while (cin >> n) { ... }

// 读入一行
string line;
getline(cin, line);

// 格式化输出
cout << fixed << setprecision(10) << ans << endl;
```

## 比赛策略

1. **读题**

   - 先看所有题目
   - 找签到题
   - 估计难度

2. **做题顺序**

   - 先做会做的
   - 不要死磕一题
   - 注意时间分配

3. **代码习惯**

   - 先想清楚再写
   - 注意边界条件
   - 样例要跑通
   - 提交前检查

4. **卡题处理**
   - 重新读题
   - 检查边界
   - 打表找规律
   - 考虑特殊情况
