#pragma once
class UnionFindSet {
 public:
  UnionFindSet(int nNodeCnt); /* 结点下标从 1 开始 */
  ~UnionFindSet();
  void Union(int nodeA, int nodeB); /* 合并两个集合 */
  bool Judge(int nodeA, int nodeB); /* 检测两个点是否在同一个集合 */
  int Find(int nodeA);     /* 找到某一个点所在树的根节点 */
  int GetBlockCnt() const; /* 当前连通块总数 */

 private:
  void Init(); /* 并查集初始化 */

  int* fa = nullptr;
  int mNodeCnt;
  int mBlockCnt; /* 连通块的个数 */
};
