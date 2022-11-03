#pragma once

#define GAME_GRAPH_MAX_WIDTH  (40)
#define GAME_GRAPH_MAX_HEIGHT (40)
#define GAME_GRAPH_MAX_POS    (GAME_GRAPH_MAX_WIDTH * GAME_GRAPH_MAX_HEIGHT)

struct GameGraphEdges {
	bool hasLeft; /* ��ǰλ������Ƿ���ǽ */
	bool hasTop;  /* ��ǰλ���Ϸ��Ƿ���ǽ */

	void DebugShow() const {
		std::cerr << "(" << (int)hasLeft <<", " << (int)hasTop << ") ";
	}
};

class TcpData;
class GameGraph
{
public:
	GameGraph();

	void SetSize(int nHeight, int nWidth, double alpha);
	void GenerateRandMap(double alpha);
	void GetTcpData(TcpData* tcpDataGet) const; /* ����ͼ����Ϊ���������� */

	bool InGraph(int posX, int posY) const;
	void DebugShow() const;

	const GameGraphEdges& GetPos(int posX, int posY) const;

private:
	void GenerateRandMap(GameGraphEdges(*pGameGraph)[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH]); /* ���������ͼ */

	int mHeight, mWidth;

	int GetNodeId(int posX, int posY) const;           /* pos ���㿪ʼ������ NodeId �� 1 ��ʼ */
	void GetNodePos(int nodeId, int& x, int& y) const; /* ��ԭ������ */

	/* ��¼����ǽ��λ�� */
	GameGraphEdges mGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];
};
