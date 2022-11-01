#pragma once

#define GAME_GRAPH_MAX_WIDTH  (40)
#define GAME_GRAPH_MAX_HEIGHT (40)
#define GAME_GRAPH_MAX_POS    (GAME_GRAPH_MAX_WIDTH * GAME_GRAPH_MAX_HEIGHT)

struct GameGraphEdges {
	bool hasLeft; /* 当前位置左侧是否有墙 */
	bool hasTop;  /* 当前位置上方是否有墙 */
};

class TcpData;
class GameGraph
{
public:
	GameGraph();

	void SetSize(int nHeight, int nWidth);
	void GenerateRandMap(); /* 生成随机地图 */
	void GetTcpData(TcpData* tcpDataGet); /* 将地图导出为二进制数据 */

private:
	int mHeight, mWidth;

	int GetNodeId(int posX, int posY) const; /* pos 从零开始，但是 NodeId 从 1 开始 */

	/* 记录所有墙的位置 */
	GameGraphEdges mGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];
};
