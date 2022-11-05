#pragma once
class UnionFindSet {
 public:
  UnionFindSet(int nNodeCnt); /* ����±�� 1 ��ʼ */
  ~UnionFindSet();
  void Union(int nodeA, int nodeB); /* �ϲ��������� */
  bool Judge(int nodeA, int nodeB); /* ����������Ƿ���ͬһ������ */
  int Find(int nodeA);     /* �ҵ�ĳһ�����������ĸ��ڵ� */
  int GetBlockCnt() const; /* ��ǰ��ͨ������ */

 private:
  void Init(); /* ���鼯��ʼ�� */

  int* fa = nullptr;
  int mNodeCnt;
  int mBlockCnt; /* ��ͨ��ĸ��� */
};
