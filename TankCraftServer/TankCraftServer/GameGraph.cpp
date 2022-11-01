#include <cassert>
#include <cstring>

#include "GameGraph.h"

GameGraph::GameGraph()
{
	mHeight = mWidth = 0;
	memset(mGameGraph, 0x00, sizeof(mGameGraph));
}

void GameGraph::SetSize(int nHeight, int nWidth)
{
	assert(nHeight > 0 && nHeight % 4 == 0 && nHeight < GAME_GRAPH_MAX_HEIGHT);
	assert(nWidth > 0 && nWidth % 4 == 0 && nWidth < GAME_GRAPH_MAX_WIDTH);

	mHeight = nHeight;
	mWidth = nWidth;

	GenerateRandMap();
}

void GameGraph::GenerateRandMap()
{
	if (mHeight == 0 || mWidth == 0) {
		return; /* 拒绝生成地图 */
	}

	/* 克鲁斯卡尔算法，最小生成树 */
	assert(false); // 没有实现完

	for (int i = 0; i < mHeight; i += 1) {
		for (int j = 0; j < mWidth; j += 1) {

		}
	}
}

int GameGraph::GetNodeId(int posX, int posY) const
{
	assert(0 <= posX && posX < mHeight);
	assert(0 <= posY && posY < mWidth);

	return posX * mWidth + posY + 1; /* 下标从 1 开始 */
}
