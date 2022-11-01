#pragma once

#define GAME_GRAPH_MAX_WIDTH  (40)
#define GAME_GRAPH_MAX_HEIGHT (40)
#define GAME_GRAPH_MAX_POS    (GAME_GRAPH_MAX_WIDTH * GAME_GRAPH_MAX_HEIGHT)

struct GameGraphEdges {
	bool hasLeft; /* ��ǰλ������Ƿ���ǽ */
	bool hasTop;  /* ��ǰλ���Ϸ��Ƿ���ǽ */
};

class TcpData;
class GameGraph
{
public:
	GameGraph();

	void SetSize(int nHeight, int nWidth);
	void GenerateRandMap(); /* ���������ͼ */
	void GetTcpData(TcpData* tcpDataGet); /* ����ͼ����Ϊ���������� */

private:
	int mHeight, mWidth;

	int GetNodeId(int posX, int posY) const; /* pos ���㿪ʼ������ NodeId �� 1 ��ʼ */

	/* ��¼����ǽ��λ�� */
	GameGraphEdges mGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];
};
