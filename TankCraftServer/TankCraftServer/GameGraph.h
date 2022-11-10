#pragma once

#define GAME_GRAPH_MAX_WIDTH (40)
#define GAME_GRAPH_MAX_HEIGHT (40)
#define GAME_GRAPH_MAX_POS (GAME_GRAPH_MAX_WIDTH * GAME_GRAPH_MAX_HEIGHT)

struct GameGraphEdges {
  bool hasLeft; /* ��ǰλ������Ƿ���ǽ */
  bool hasTop;  /* ��ǰλ���Ϸ��Ƿ���ǽ */

  void DebugShow() const;
};

class TcpData;
class GameGraph {
 public:
  GameGraph();

  void SetSize(int nHeight, int nWidth, double alpha);
  void GetTcpData(TcpData* tcpDataGet) const; /* ����ͼ����Ϊ���������� */

  bool InGraphGrid(int posX, int posY) const;
  void DebugShow() const;

  const GameGraphEdges& GetPos(int posX, int posY) const;
  double GetCreateTime() const; /* �����ͼ����ʱ�� */

  int GetHeight() const; /* ��ȡ��ͼ��ص���Ϣ */
  int GetWidth() const;

  /* ʵ��������� posX ���� posY ���µģ����ͼ������ͬ */
  bool InGraph(double posX, double posY,
               double r) const; /* �ж�һ��Բ�Ƿ���ȫ�ڵ�ͼ�� */

  void BoxFit(double* posX, double* posY, double r) const; /* ʹ�ñ߽�Լ��һ��̹�� */
  
  /* ���ߵĴ����� */
  bool PosHasTopEdge(int gridX, int gridY) const;
  bool PosHasBottomEdge(int gridX, int gridY) const;
  bool PosHasLeftEdge(int gridX, int gridY) const;
  bool PosHasRightEdge(int gridX, int gridY) const;

  /* ���ǵĴ����� */
  bool PosHasTopLeft(int gridX, int gridY) const;
  bool PosHasTopRight(int gridX, int gridY) const;
  bool PosHasBottomLeft(int gridX, int gridY) const;
  bool PosHasBottomRight(int gridX, int gridY) const;

 private:
  void GenerateRandMap(double alpha);
  void GenerateRandMap(
      GameGraphEdges (*pGameGraph)[GAME_GRAPH_MAX_HEIGHT]
                                  [GAME_GRAPH_MAX_WIDTH]); /* ���������ͼ */

  int mHeight, mWidth;
  double mCreateTime;

  int GetNodeId(int posX,
                int posY) const; /* pos ���㿪ʼ������ NodeId �� 1 ��ʼ */
  void GetNodePos(int nodeId, int& x, int& y) const; /* ��ԭ������ */

  /* ��¼����ǽ��λ�� */
  GameGraphEdges mGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];
};
