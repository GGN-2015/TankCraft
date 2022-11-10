#pragma once

#define GAME_GRAPH_MAX_WIDTH (40)
#define GAME_GRAPH_MAX_HEIGHT (40)
#define GAME_GRAPH_MAX_POS (GAME_GRAPH_MAX_WIDTH * GAME_GRAPH_MAX_HEIGHT)

struct GameGraphEdges {
  bool hasLeft; /* 当前位置左侧是否有墙 */
  bool hasTop;  /* 当前位置上方是否有墙 */

  void DebugShow() const;
};

class TcpData;
class GameGraph {
 public:
  GameGraph();

  void SetSize(int nHeight, int nWidth, double alpha);
  void GetTcpData(TcpData* tcpDataGet) const; /* 将地图导出为二进制数据 */

  bool InGraphGrid(int posX, int posY) const;
  void DebugShow() const;

  const GameGraphEdges& GetPos(int posX, int posY) const;
  double GetCreateTime() const; /* 计算地图创建时间 */

  int GetHeight() const; /* 获取地图相关的信息 */
  int GetWidth() const;

  /* 实体的坐标是 posX 向右 posY 向下的，与地图描述不同 */
  bool InGraph(double posX, double posY,
               double r) const; /* 判断一个圆是否完全在地图中 */

  void BoxFit(double* posX, double* posY, double r) const; /* 使用边界约束一个坦克 */
  
  /* 检查边的存在性 */
  bool PosHasTopEdge(int gridX, int gridY) const;
  bool PosHasBottomEdge(int gridX, int gridY) const;
  bool PosHasLeftEdge(int gridX, int gridY) const;
  bool PosHasRightEdge(int gridX, int gridY) const;

  /* 检查角的存在性 */
  bool PosHasTopLeft(int gridX, int gridY) const;
  bool PosHasTopRight(int gridX, int gridY) const;
  bool PosHasBottomLeft(int gridX, int gridY) const;
  bool PosHasBottomRight(int gridX, int gridY) const;

 private:
  void GenerateRandMap(double alpha);
  void GenerateRandMap(
      GameGraphEdges (*pGameGraph)[GAME_GRAPH_MAX_HEIGHT]
                                  [GAME_GRAPH_MAX_WIDTH]); /* 生成随机地图 */

  int mHeight, mWidth;
  double mCreateTime;

  int GetNodeId(int posX,
                int posY) const; /* pos 从零开始，但是 NodeId 从 1 开始 */
  void GetNodePos(int nodeId, int& x, int& y) const; /* 还原回坐标 */

  /* 记录所有墙的位置 */
  GameGraphEdges mGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];
};
