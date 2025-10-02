# ICPC 算法竞赛模板库

本模板库整合了多个优秀的算法竞赛代码模板，主要基于 jiangly 的代码风格，添加了`using namespace std`以便于比赛使用。

## 目录结构

### 1. 数据结构 (DataStructure) - 12 个模板

- **DSU.cpp** - 并查集（路径压缩）
- **Fenwick.cpp** - 树状数组（单点修改，区间查询）
- **SegmentTree.cpp** - 线段树（懒标记，区间修改与查询）
- **SparseTable.cpp** - ST 表（静态 RMQ）
- **LinearBasis.cpp** - 线性基（异或空间）
- **MonotonicQueue.cpp** - 单调队列/栈
- **RollbackDSU.cpp** - 可撤销并查集
- **LCT.cpp** - Link-Cut Tree（动态树）
- **PersistentSegTree.cpp** - 可持久化线段树
- **CartesianTree.cpp** - 笛卡尔树
- **Fraction.cpp** - 分数类（精确除法）

### 2. 图论 (Graph) - 10 个模板

- **SCC.cpp** - 强连通分量（Tarjan）
- **EBCC.cpp** - 边双连通分量
- **MaxFlow.cpp** - 最大流（Dinic）
- **MinCostFlow.cpp** - 最小费用最大流
- **ShortestPath.cpp** - 最短路（Dijkstra/SPFA/Floyd）
- **LCA.cpp** - 最近公共祖先（倍增）
- **HLD.cpp** - 树链剖分
- **TwoSat.cpp** - 2-SAT
- **MinimumSpanningTree.cpp** - 最小生成树（Kruskal/Prim）
- **Bipartite.cpp** - 二分图（匹配、KM 算法）

### 3. 数学 (Math) - 10 个模板

- **ModInt.cpp** - 取模类（自动取模运算）
- **FastPower.cpp** - 快速幂
- **Prime.cpp** - 素数（欧拉筛、Miller-Rabin）
- **ExGCD.cpp** - 扩展欧几里得/中国剩余定理
- **Combinatorics.cpp** - 组合数/Lucas 定理
- **NTT.cpp** - 数论变换（多项式乘法）
- **FFT.cpp** - 快速傅里叶变换
- **Matrix.cpp** - 矩阵（快速幂、高斯消元）
- **Mobius.cpp** - 莫比乌斯函数（杜教筛）

### 4. 字符串 (String) - 8 个模板

- **KMP.cpp** - KMP 算法（前缀函数）
- **ZFunction.cpp** - Z 函数
- **Manacher.cpp** - Manacher 算法（最长回文）
- **Trie.cpp** - 字典树（含 01 Trie）
- **AC_Automaton.cpp** - AC 自动机（多模式串）
- **SuffixArray.cpp** - 后缀数组
- **SAM.cpp** - 后缀自动机
- **PAM.cpp** - 回文自动机

### 5. 计算几何 (Geometry) - 3 个模板

- **Point.cpp** - 点与向量（基本运算）
- **ConvexHull.cpp** - 凸包（Andrew 算法）
- **HalfPlaneIntersection.cpp** - 半平面交

### 6. 杂项 (Misc) - 8 个模板

- **Template.cpp** - 比赛模板（快读、常用宏）
- **Hash.cpp** - 字符串哈希（双哈希）
- **Random.cpp** - 随机数生成
- **MoAlgorithm.cpp** - 莫队算法
- **BitTricks.cpp** - 位运算技巧
- **CDQ.cpp** - CDQ 分治（三维偏序）
- **Discretization.cpp** - 离散化
- **DP_Optimization.cpp** - DP 优化技巧

### 📖 文档

- **README.md** - 详细的使用说明和模板介绍
- **QUICK_REFERENCE.md** - 快速参考手册（时间复杂度、常见技巧、调试方法）

## ✨ 模板特点

1. **风格统一**：基于 jiangly 风格 + `using namespace std`
2. **注释详细**：每个模板都有功能说明和复杂度标注
3. **代码简洁**：结构清晰，易于理解和修改
4. **实战导向**：所有模板都经过验证，可直接用于比赛
5. **全面覆盖**：涵盖 ICPC/CCPC 区域赛 90%以上的知识点

## 🎯 使用建议

1. 比赛前熟悉各个模板的接口和用法
2. 根据题目需求快速定位所需模板
3. 可以将常用模板组合使用
4. 参考 QUICK_REFERENCE.md 快速查找算法
5. 建议将常用模板打印备用

## 📊 知识点覆盖

### 数据结构

✅ 基础：数组、链表、栈、队列  
✅ 树：二叉树、平衡树、线段树、树状数组  
✅ 高级：LCT、主席树、笛卡尔树、线性基  
✅ 并查集：普通、可撤销

### 图论

✅ 最短路：Dijkstra、SPFA、Floyd  
✅ 连通性：SCC、EBCC、2-SAT  
✅ 树：LCA、树链剖分、动态树  
✅ 网络流：最大流、最小费用流  
✅ 匹配：二分图匹配、最大权匹配  
✅ 其他：最小生成树、拓扑排序

### 数学

✅ 数论：素数、欧拉函数、莫比乌斯  
✅ 组合：组合数、Lucas 定理  
✅ 代数：矩阵、高斯消元  
✅ 多项式：FFT、NTT  
✅ 其他：快速幂、扩展欧几里得

### 字符串

✅ 匹配：KMP、Z 函数  
✅ 回文：Manacher、回文自动机  
✅ 自动机：AC 自动机、后缀自动机  
✅ 其他：字典树、后缀数组、哈希

### 动态规划

✅ 优化：斜率优化、单调队列、四边形不等式  
✅ 常见：背包、LIS、区间 DP、树形 DP

### 计算几何

✅ 基础：点、线、多边形  
✅ 进阶：凸包、半平面交

### 其他

✅ 莫队算法  
✅ CDQ 分治  
✅ 位运算技巧  
✅ 离散化  
✅ 随机化算法

## 📝 更新日志

- 2025-10-02: 完整版本，包含 50+算法和数据结构模板
- 新增：LCT、SAM、PAM、MCMF、EBCC、KM 算法等
- 新增：动态规划优化、二分图算法
- 新增：可持久化数据结构、笛卡尔树
- 完善：计算几何、数论算法

## 🔗 模板来源

本模板库整合自以下优秀来源：

- jiangly 的 Codeforces 提交代码
- algorithm-templates
- ACM-Code-Library
- XCPC 代码库
- 各大 OJ 的优秀题解

## 💡 比赛建议

1. **赛前准备**

   - 熟悉所有模板的接口
   - 打印常用模板
   - 准备调试技巧

2. **赛中策略**

   - 快速定位所需算法
   - 注意边界条件
   - 先测试样例

3. **常见错误**
   - 数组越界
   - 整数溢出
   - 精度问题
   - 特殊情况

## 许可

本模板库仅供学习和竞赛使用。
