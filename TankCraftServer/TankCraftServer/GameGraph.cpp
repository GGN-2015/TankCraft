#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "GameGraph.h"
#include "UnionFindSet.h"
#include "Utils.h"

GameGraph::GameGraph()
{
	mHeight = mWidth = 0;
	memset(mGameGraph, 0x00, sizeof(mGameGraph));
}

void GameGraph::SetSize(int nHeight, int nWidth, double alpha)
{
	/* ��Э��Ҫ���ͼ�ߴ��� 8 �ı��� */
	assert(nHeight > 0 && nHeight % 8 == 0 && nHeight <= GAME_GRAPH_MAX_HEIGHT);
	assert(nWidth > 0 && nWidth % 8 == 0 && nWidth <= GAME_GRAPH_MAX_WIDTH);

	mHeight = nHeight;
	mWidth = nWidth;

	GenerateRandMap(alpha);
}

void GameGraph::GenerateRandMap(double alpha)
{
	assert(0.0 <= alpha && alpha <= 1.0);

	int getTime = clock();
	GameGraphEdges nGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];

	/* �������ݵ�ͼ */
	GenerateRandMap(&nGameGraph);
	GenerateRandMap(&mGameGraph);

	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 1) {
			if (Utils::GetRandomDouble() <= alpha) {
				mGameGraph[i][j].hasLeft = mGameGraph[i][j].hasLeft && nGameGraph[i][j].hasLeft;
			}
			if (Utils::GetRandomDouble() <= alpha) {
				mGameGraph[i][j].hasTop = mGameGraph[i][j].hasTop && nGameGraph[i][j].hasTop;
			}
		}
	}
	int endTime = clock();

	std::cerr << "GameGraph::GenerateRandMap() timeCost = " << (endTime - getTime) << std::endl;
}

struct EdgeMsg { /* ��¼ Kruscal �㷨��Ҫʹ�õı� */
	int nodeIdA, nodeIdB;
	long long randVal;
};

/* ���ڸ���������� */
static bool edgeCmp(const EdgeMsg& e1, const EdgeMsg& e2) {
	return e1.randVal < e2.randVal;
}

void GameGraph::GenerateRandMap(GameGraphEdges (*pGameGraph)[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH])
{
	if (mHeight == 0 || mWidth == 0) {
		return; /* �ܾ����ɵ�ͼ */
	}

	std::vector<EdgeMsg> allEdges;                /* ��¼���б� */
	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 1) {
			(*pGameGraph)[i][j].hasLeft = true; /* �����б߶���� */
			(*pGameGraph)[i][j].hasTop = true;
		}
	}

	/* ÿ���������������� */
	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 1) {
			if (InGraph(i, j - 1)) { /* �������� */
				allEdges.push_back({ GetNodeId(i, j), GetNodeId(i, j - 1), Utils::GetRandLongLong() });
			}
			if (InGraph(i - 1, j)) { /* �������� */
				allEdges.push_back({ GetNodeId(i, j), GetNodeId(i - 1, j), Utils::GetRandLongLong() });
			}
		}
	}

	/* ������� */
	std::sort(allEdges.begin(), allEdges.end(), edgeCmp);

	UnionFindSet ufs(mHeight * mWidth); /* ʹ�ò��鼯ʵ�� Kruscal */
	int pos;
	for (pos = 0; pos < (int)allEdges.size() && ufs.GetBlockCnt() > 1; pos += 1) {
		
		int nodeA = allEdges[pos].nodeIdA;
		int nodeB = allEdges[pos].nodeIdB;

		if (!ufs.Judge(nodeA, nodeB)) { /* ����������㻹����һ����ͨ�� */
			ufs.Union(nodeA, nodeB);

			int x, y; GetNodePos(nodeA, x, y);

			if (nodeB == nodeA - 1) { /* nodeB �� nodeA ��� */
				(*pGameGraph)[x][y].hasLeft = false;
			}
			else if (nodeB == nodeA - mWidth) { /* nodeB �� nodeA �Ϸ� */
				(*pGameGraph)[x][y].hasTop = false;
			}
			else {
				assert(false); /* �����˷Ƿ��ı� */
			}
		}
	}
}

bool GameGraph::InGraph(int posX, int posY) const
{
	return
		0 <= posX && posX < mHeight &&
		0 <= posY && posY < mWidth;
}

void GameGraph::DebugShow() const
{
	std::cerr << "GameGraph::DebugShow() " << std::endl;
	for (int i = 0; i < mHeight; i += 1) {
		std::cerr << "    ";
		for (int j = 0; j < mWidth; j += 1) {
			mGameGraph[i][j].DebugShow();
		}
		std::cerr << std::endl;
	}
	std::cerr << std::endl;
}

const GameGraphEdges& GameGraph::GetPos(int posX, int posY) const
{
	assert(InGraph(posX, posY));

	return mGameGraph[posX][posY];
}

int GameGraph::GetNodeId(int posX, int posY) const
{
	assert(InGraph(posX, posY));

	return posX * mWidth + posY + 1; /* �±�� 1 ��ʼ */
}

void GameGraph::GetNodePos(int nodeId, int& x, int& y) const
{
	nodeId--;
	y = nodeId % mWidth;
	x = nodeId / mWidth;

	assert(InGraph(x, y));
}
