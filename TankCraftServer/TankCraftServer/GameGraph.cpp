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

	SetSize(8, 8, 0.4);
}

void GameGraph::SetSize(int nHeight, int nWidth, double alpha)
{
	/* 新协议要求地图尺寸是 8 的倍数 */
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

	/* 生成两份地图 */
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
	mCreateTime = Utils::GetClockTime(); /* 记录新地图的创建时间 */
}

void GameGraph::GetTcpData(TcpData* tcpDataGet) const
{
	int bitCnt = mHeight * mWidth * 2;
	assert(bitCnt % 8 == 0 && mWidth % 4 == 0);

	int byteCnt = bitCnt / 8;

	int tcpDataLen = byteCnt + 6;
	char* buf = new char[tcpDataLen];

	Utils::DumpUnsignedShortToBuffer(buf, 0, 2);                       /* 2 号消息表示地图消息 */
	Utils::DumpUnsignedShortToBuffer(buf, 2, byteCnt + 2);             /* 数据部分长度 */
	Utils::DumpUnsignedShortToBuffer(buf, 4, mHeight | (mWidth << 8)); /* 地图尺寸 */

	int pos = 6;

	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 4) {
			char val = Utils::GetCharFromBools(
				mGameGraph[i][j + 0].hasLeft ,
				mGameGraph[i][j + 0].hasTop  ,
				mGameGraph[i][j + 1].hasLeft ,
				mGameGraph[i][j + 1].hasTop  ,
				mGameGraph[i][j + 2].hasLeft ,
				mGameGraph[i][j + 2].hasTop  ,
				mGameGraph[i][j + 3].hasLeft ,
				mGameGraph[i][j + 3].hasTop
			);

			if (pos < tcpDataLen) { /* 避免缓冲区溢出? */
				buf[pos++] = (char)val;
			}
			else {
				assert(false);
			}
		}
	}
	assert(pos == tcpDataLen);

	tcpDataGet->SetData(buf, tcpDataLen);
	delete[] buf;
}

struct EdgeMsg { /* 记录 Kruscal 算法需要使用的边 */
	int nodeIdA, nodeIdB;
	long long randVal;
};

/* 用于给边随机排序 */
static bool edgeCmp(const EdgeMsg& e1, const EdgeMsg& e2) {
	return e1.randVal < e2.randVal;
}

void GameGraph::GenerateRandMap(GameGraphEdges (*pGameGraph)[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH])
{
	if (mHeight == 0 || mWidth == 0) {
		return; /* 拒绝生成地图 */
	}

	std::vector<EdgeMsg> allEdges;                /* 记录所有边 */
	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 1) {
			(*pGameGraph)[i][j].hasLeft = true; /* 把所有边都阻断 */
			(*pGameGraph)[i][j].hasTop = true;
		}
	}

	/* 每个点向左向上连边 */
	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 1) {
			if (InGraph(i, j - 1)) { /* 向左连边 */
				allEdges.push_back({ GetNodeId(i, j), GetNodeId(i, j - 1), Utils::GetRandLongLong() });
			}
			if (InGraph(i - 1, j)) { /* 向上连边 */
				allEdges.push_back({ GetNodeId(i, j), GetNodeId(i - 1, j), Utils::GetRandLongLong() });
			}
		}
	}

	/* 随机排序 */
	std::sort(allEdges.begin(), allEdges.end(), edgeCmp);

	UnionFindSet ufs(mHeight * mWidth); /* 使用并查集实现 Kruscal */
	int pos;
	for (pos = 0; pos < (int)allEdges.size() && ufs.GetBlockCnt() > 1; pos += 1) {
		
		int nodeA = allEdges[pos].nodeIdA;
		int nodeB = allEdges[pos].nodeIdB;

		if (!ufs.Judge(nodeA, nodeB)) { /* 如果这两个点还不在一个连通块 */
			ufs.Union(nodeA, nodeB);

			int x, y; GetNodePos(nodeA, x, y);

			if (nodeB == nodeA - 1) { /* nodeB 在 nodeA 左侧 */
				(*pGameGraph)[x][y].hasLeft = false;
			}
			else if (nodeB == nodeA - mWidth) { /* nodeB 在 nodeA 上方 */
				(*pGameGraph)[x][y].hasTop = false;
			}
			else {
				assert(false); /* 出现了非法的边 */
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

double GameGraph::GetCreateTime() const
{
	return mCreateTime;
}

int GameGraph::GetNodeId(int posX, int posY) const
{
	assert(InGraph(posX, posY));

	return posX * mWidth + posY + 1; /* 下标从 1 开始 */
}

void GameGraph::GetNodePos(int nodeId, int& x, int& y) const
{
	nodeId--;
	y = nodeId % mWidth;
	x = nodeId / mWidth;

	assert(InGraph(x, y));
}

void GameGraphEdges::DebugShow() const
{
	std::cerr << "(" << (int)hasLeft << ", " << (int)hasTop << ") ";
}
