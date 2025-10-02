# 模板索引

快速查找所需模板

## 📊 统计信息

**总计：49 个 C++模板文件**

- 数据结构：11 个
- 图论：10 个
- 数学：10 个
- 字符串：8 个
- 计算几何：3 个
- 杂项：7 个

---

## 🔍 按功能分类

### 数据结构 (DataStructure)

| 模板                  | 功能           | 复杂度   | 关键词              |
| --------------------- | -------------- | -------- | ------------------- |
| DSU.cpp               | 并查集         | O(α(n))  | 连通性、合并        |
| Fenwick.cpp           | 树状数组       | O(log n) | 单点修改、区间和    |
| SegmentTree.cpp       | 线段树         | O(log n) | 区间修改、区间查询  |
| SparseTable.cpp       | ST 表          | O(1)查询 | 静态 RMQ            |
| LCT.cpp               | Link-Cut Tree  | O(log n) | 动态树、路径查询    |
| LinearBasis.cpp       | 线性基         | -        | 异或、子集          |
| MonotonicQueue.cpp    | 单调队列       | O(n)     | 滑动窗口最值        |
| RollbackDSU.cpp       | 可撤销并查集   | O(log n) | 动态连通性          |
| PersistentSegTree.cpp | 可持久化线段树 | O(log n) | 历史版本、区间 k 小 |
| CartesianTree.cpp     | 笛卡尔树       | O(n)     | RMQ、最大矩形       |
| Fraction.cpp          | 分数类         | -        | 精确除法            |

### 图论 (Graph)

| 模板                    | 功能         | 复杂度     | 关键词              |
| ----------------------- | ------------ | ---------- | ------------------- |
| SCC.cpp                 | 强连通分量   | O(n+m)     | 有向图、缩点        |
| EBCC.cpp                | 边双连通分量 | O(n+m)     | 割边、桥            |
| MaxFlow.cpp             | 最大流       | O(n²m)     | Dinic、网络流       |
| MinCostFlow.cpp         | 最小费用流   | O(nmf)     | 费用流              |
| ShortestPath.cpp        | 最短路       | 各异       | Dijkstra/SPFA/Floyd |
| LCA.cpp                 | 最近公共祖先 | O(log n)   | 倍增                |
| HLD.cpp                 | 树链剖分     | O(log n)   | 树上路径            |
| TwoSat.cpp              | 2-SAT        | O(n+m)     | 布尔方程            |
| MinimumSpanningTree.cpp | 最小生成树   | O(m log m) | Kruskal/Prim        |
| Bipartite.cpp           | 二分图       | O(nm)      | 匹配、KM            |

### 数学 (Math)

| 模板              | 功能         | 复杂度          | 关键词             |
| ----------------- | ------------ | --------------- | ------------------ |
| ModInt.cpp        | 取模类       | -               | 自动取模           |
| FastPower.cpp     | 快速幂       | O(log n)        | 幂运算             |
| Prime.cpp         | 素数         | O(n)/O(k log³n) | 筛法、Miller-Rabin |
| ExGCD.cpp         | 扩展欧几里得 | O(log n)        | 逆元、CRT          |
| Combinatorics.cpp | 组合数       | 各异            | C(n,m)、Lucas      |
| NTT.cpp           | 数论变换     | O(n log n)      | 多项式乘法         |
| FFT.cpp           | 快速傅里叶   | O(n log n)      | 实数卷积           |
| Matrix.cpp        | 矩阵         | O(n³)           | 快速幂、高斯消元   |
| Mobius.cpp        | 莫比乌斯     | O(n)            | 杜教筛、反演       |

### 字符串 (String)

| 模板             | 功能       | 复杂度     | 关键词         |
| ---------------- | ---------- | ---------- | -------------- |
| KMP.cpp          | KMP 算法   | O(n+m)     | 前缀函数、匹配 |
| ZFunction.cpp    | Z 函数     | O(n)       | 最长公共前缀   |
| Manacher.cpp     | Manacher   | O(n)       | 最长回文       |
| Trie.cpp         | 字典树     | O(L)       | 前缀、01Trie   |
| AC_Automaton.cpp | AC 自动机  | O(n+m)     | 多模式匹配     |
| SuffixArray.cpp  | 后缀数组   | O(n log n) | 后缀、LCP      |
| SAM.cpp          | 后缀自动机 | O(n)       | 子串识别       |
| PAM.cpp          | 回文自动机 | O(n)       | 回文串         |

### 计算几何 (Geometry)

| 模板                      | 功能     | 复杂度     | 关键词           |
| ------------------------- | -------- | ---------- | ---------------- |
| Point.cpp                 | 点与向量 | O(1)       | 基本运算         |
| ConvexHull.cpp            | 凸包     | O(n log n) | Andrew、旋转卡壳 |
| HalfPlaneIntersection.cpp | 半平面交 | O(n log n) | 凸多边形         |

### 杂项 (Misc)

| 模板                | 功能       | 复杂度     | 关键词         |
| ------------------- | ---------- | ---------- | -------------- |
| Template.cpp        | 比赛模板   | -          | 快读、宏定义   |
| Hash.cpp            | 字符串哈希 | O(n)       | 双哈希、子串   |
| Random.cpp          | 随机数     | O(1)       | mt19937        |
| MoAlgorithm.cpp     | 莫队       | O(n√n)     | 离线查询       |
| BitTricks.cpp       | 位运算     | -          | 技巧集合       |
| CDQ.cpp             | CDQ 分治   | O(n log²n) | 三维偏序       |
| Discretization.cpp  | 离散化     | O(n log n) | 坐标压缩       |
| DP_Optimization.cpp | DP 优化    | 各异       | 斜率、单调队列 |

---

## 🎯 按问题类型查找

### 区间查询问题

- 静态区间最值 → **SparseTable.cpp**
- 动态区间和 → **Fenwick.cpp** / **SegmentTree.cpp**
- 区间最值+修改 → **SegmentTree.cpp**
- 滑动窗口最值 → **MonotonicQueue.cpp**
- 历史版本查询 → **PersistentSegTree.cpp**

### 图的连通性

- 判断连通 → **DSU.cpp**
- 强连通分量 → **SCC.cpp**
- 边双连通 → **EBCC.cpp**
- 动态连通性 → **LCT.cpp**
- 可撤销操作 → **RollbackDSU.cpp**

### 最短路问题

- 单源非负权 → **Dijkstra** (ShortestPath.cpp)
- 单源有负权 → **SPFA** (ShortestPath.cpp)
- 全源最短路 → **Floyd** (ShortestPath.cpp)

### 字符串匹配

- 单模式串 → **KMP.cpp** / **ZFunction.cpp**
- 多模式串 → **AC_Automaton.cpp**
- 子串查找 → **Hash.cpp**
- 回文串 → **Manacher.cpp** / **PAM.cpp**

### 数论问题

- 质数判定 → **Prime.cpp** (Miller-Rabin)
- 分解质因数 → **Prime.cpp**
- 求逆元 → **ExGCD.cpp** / **ModInt.cpp**
- 组合数 → **Combinatorics.cpp**
- 中国剩余定理 → **ExGCD.cpp**

### 动态规划优化

- 决策单调性 → **DP_Optimization.cpp**
- 斜率优化 → **DP_Optimization.cpp**
- 单调队列 → **DP_Optimization.cpp** / **MonotonicQueue.cpp**
- 四边形不等式 → **DP_Optimization.cpp**

### 网络流问题

- 最大流 → **MaxFlow.cpp**
- 最小费用流 → **MinCostFlow.cpp**
- 二分图匹配 → **Bipartite.cpp**
- 最大权匹配 → **Bipartite.cpp** (KM 算法)

---

## 💡 常见题型对应

| 题型           | 推荐模板                   |
| -------------- | -------------------------- |
| 区间第 k 小    | PersistentSegTree.cpp      |
| 最大矩形面积   | CartesianTree.cpp          |
| 树上路径查询   | LCA.cpp / HLD.cpp          |
| 动态维护连通性 | LCT.cpp                    |
| 异或相关       | LinearBasis.cpp / Trie.cpp |
| 多项式乘法     | NTT.cpp / FFT.cpp          |
| 三维偏序       | CDQ.cpp                    |
| 莫比乌斯反演   | Mobius.cpp                 |
| 2-SAT 问题     | TwoSat.cpp                 |

---

## ⚡ 快速查找

**Ctrl+F 搜索关键词：**

- 并查集 DSU
- 线段树 SegmentTree
- 最短路 Shortest
- 字符串匹配 KMP AC
- 网络流 Flow
- 凸包 Convex
- 动态规划 DP
- 数论 Prime Mod
