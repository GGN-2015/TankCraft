#include <cassert>

#include "UnionFindSet.h"

UnionFindSet::UnionFindSet(int nNodeCnt)
{
	mNodeCnt = mBlockCnt = nNodeCnt;
	fa = new int[(long long)mNodeCnt + 1];

	Init(); /* 将每个结点的父亲初始化成自己 */
}

UnionFindSet::~UnionFindSet()
{
	delete[] fa;
	fa = nullptr;
}

void UnionFindSet::Union(int nodeA, int nodeB)
{
	assert(1 <= nodeA && nodeA <= mNodeCnt);
	assert(1 <= nodeB && nodeB <= mNodeCnt);

	nodeA = Find(nodeA);
	nodeB = Find(nodeB);

	if (nodeA != nodeB) { /* 认贼作父 */
		mBlockCnt -= 1;
		fa[nodeA] = nodeB;
	}
}

bool UnionFindSet::Judge(int nodeA, int nodeB)
{
	return Find(nodeA) == Find(nodeB);
}

int UnionFindSet::Find(int nodeA)
{
	assert(1 <= nodeA && nodeA <= mNodeCnt);

	if (fa[nodeA] == nodeA)
		return nodeA;
	else {
		fa[nodeA] = Find(fa[nodeA]);
		return fa[nodeA];
	}
}

int UnionFindSet::GetBlockCnt() const
{
	return mBlockCnt;
}

void UnionFindSet::Init()
{
	assert(fa != nullptr);

	for (int i = 0; i <= mNodeCnt; i += 1) {
		fa[i] = i;
	}
}
