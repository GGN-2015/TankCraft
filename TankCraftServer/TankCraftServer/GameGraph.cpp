#include "GameGraph.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "UnionFindSet.h"
#include "Utils.h"

GameGraph::GameGraph() {
  mHeight = mWidth = 0;
  memset(mGameGraph, 0x00, sizeof(mGameGraph));

  SetSize(8, 8, 0.4);
}

void GameGraph::SetSize(int nHeight, int nWidth, double alpha) {
  /* ��Э��Ҫ���ͼ�ߴ��� 8 �ı��� */
  assert(nHeight > 0 && nHeight % 8 == 0 && nHeight <= GAME_GRAPH_MAX_HEIGHT);
  assert(nWidth > 0 && nWidth % 8 == 0 && nWidth <= GAME_GRAPH_MAX_WIDTH);

  mHeight = nHeight;
  mWidth = nWidth;

  GenerateRandMap(alpha);
}

void GameGraph::GenerateRandMap(double alpha) {
  assert(0.0 <= alpha && alpha <= 1.0);

  int getTime = clock();
  GameGraphEdges nGameGraph[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH];

  /* �������ݵ�ͼ */
  GenerateRandMap(&nGameGraph);
  GenerateRandMap(&mGameGraph);

  for (int i = 0; i < mHeight; i += 1) {
    for (int j = 0; j < mWidth; j += 1) {
      if (Utils::GetRandomDouble() <= alpha) {
        mGameGraph[i][j].hasLeft =
            mGameGraph[i][j].hasLeft && nGameGraph[i][j].hasLeft;
      }
      if (Utils::GetRandomDouble() <= alpha) {
        mGameGraph[i][j].hasTop =
            mGameGraph[i][j].hasTop && nGameGraph[i][j].hasTop;
      }
    }
  }
  int endTime = clock();

  std::cerr << "GameGraph::GenerateRandMap() timeCost = " << (endTime - getTime)
            << std::endl;
  mCreateTime = Utils::GetClockTime(); /* ��¼�µ�ͼ�Ĵ���ʱ�� */
}

void GameGraph::GetTcpData(TcpData* tcpDataGet) const {
  int bitCnt = mHeight * mWidth * 2;
  assert(bitCnt % 8 == 0 && mWidth % 4 == 0);

  int byteCnt = bitCnt / 8;

  int tcpDataLen = byteCnt + 6;
  char* buf = new char[tcpDataLen];

  Utils::DumpUnsignedShortToBuffer(buf, 0, 2); /* 2 ����Ϣ��ʾ��ͼ��Ϣ */
  Utils::DumpUnsignedShortToBuffer(buf, 2, byteCnt + 2); /* ���ݲ��ֳ��� */
  Utils::DumpUnsignedShortToBuffer(buf, 4,
                                   mHeight | (mWidth << 8)); /* ��ͼ�ߴ� */

  int pos = 6;

  for (int i = 0; i < mHeight; i += 1) {
    for (int j = 0; j < mWidth; j += 4) {
      char val = Utils::GetCharFromBools(
          mGameGraph[i][j + 0].hasLeft, mGameGraph[i][j + 0].hasTop,
          mGameGraph[i][j + 1].hasLeft, mGameGraph[i][j + 1].hasTop,
          mGameGraph[i][j + 2].hasLeft, mGameGraph[i][j + 2].hasTop,
          mGameGraph[i][j + 3].hasLeft, mGameGraph[i][j + 3].hasTop);

      if (pos < tcpDataLen) { /* ���⻺�������? */
        buf[pos++] = (char)val;
      } else {
        assert(false);
      }
    }
  }
  assert(pos == tcpDataLen);

  tcpDataGet->SetData(buf, tcpDataLen);
  delete[] buf;
}

struct EdgeMsg { /* ��¼ Kruscal �㷨��Ҫʹ�õı� */
  int nodeIdA, nodeIdB;
  long long randVal;
};

/* ���ڸ���������� */
static bool edgeCmp(const EdgeMsg& e1, const EdgeMsg& e2) {
  return e1.randVal < e2.randVal;
}

void GameGraph::GenerateRandMap(
    GameGraphEdges (*pGameGraph)[GAME_GRAPH_MAX_HEIGHT][GAME_GRAPH_MAX_WIDTH]) {
  if (mHeight == 0 || mWidth == 0) {
    return; /* �ܾ����ɵ�ͼ */
  }

  std::vector<EdgeMsg> allEdges; /* ��¼���б� */
  for (int i = 0; i < mHeight; i += 1) {
    for (int j = 0; j < mWidth; j += 1) {
      (*pGameGraph)[i][j].hasLeft = true; /* �����б߶���� */
      (*pGameGraph)[i][j].hasTop = true;
    }
  }

  /* ÿ���������������� */
  for (int i = 0; i < mHeight; i += 1) {
    for (int j = 0; j < mWidth; j += 1) {
      if (InGraphGrid(i, j - 1)) { /* �������� */
        allEdges.push_back(
            {GetNodeId(i, j), GetNodeId(i, j - 1), Utils::GetRandLongLong()});
      }
      if (InGraphGrid(i - 1, j)) { /* �������� */
        allEdges.push_back(
            {GetNodeId(i, j), GetNodeId(i - 1, j), Utils::GetRandLongLong()});
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

      int x, y;
      GetNodePos(nodeA, x, y);

      if (nodeB == nodeA - 1) { /* nodeB �� nodeA ��� */
        (*pGameGraph)[x][y].hasLeft = false;
      } else if (nodeB == nodeA - mWidth) { /* nodeB �� nodeA �Ϸ� */
        (*pGameGraph)[x][y].hasTop = false;
      } else {
        assert(false); /* �����˷Ƿ��ı� */
      }
    }
  }
}

bool GameGraph::InGraphGrid(int posX, int posY) const {
  return 0 <= posX && posX < mHeight && 0 <= posY && posY < mWidth;
}

void GameGraph::DebugShow() const {
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

const GameGraphEdges& GameGraph::GetPos(int posX, int posY) const {
  assert(InGraphGrid(posX, posY));

  return mGameGraph[posX][posY];
}

double GameGraph::GetCreateTime() const { return mCreateTime; }

int GameGraph::GetHeight() const { return mHeight; }

int GameGraph::GetWidth() const { return mWidth; }

bool GameGraph::InGraph(double posX, double posY, double r) const {
  return 0 <= posY - r && posY + r <= mHeight && 0 <= posX - r &&
         posX + r <= mWidth;
}

void GameGraph::BoxFit(double* posX, double* posY, double r) const {
  const double eps = 1e-5;
  int gridX = int(*posY + eps);
  int gridY = int(*posX + eps);

  /* �߽��� */
  if (PosHasLeftEdge(gridX, gridY)) {
    *posX = std::max(*posX, gridY + r);
  }
  if (PosHasRightEdge(gridX, gridY)) {
    *posX = std::min(*posX, gridY + 1 - r);
  }
  if (PosHasTopEdge(gridX, gridY)) {
    *posY = std::max(*posY, gridX + r);
  }
  if (PosHasBottomEdge(gridX, gridY)) {
    *posY = std::min(*posY, gridX + 1 - r);
  }

  /* ������ */
  if (PosHasTopLeft(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY, gridX, r);
  }
  if (PosHasTopRight(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY+1, gridX, r);
  }
  if (PosHasBottomLeft(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY, gridX+1, r);
  }
  if (PosHasBottomRight(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY + 1, gridX + 1, r);
  }
}

void GameGraph::BulletBoxFit(double* posX, double* posY, double r,
                             double* dirR) {
  const double eps = 1e-5;
  int gridX = int(*posY + eps);
  int gridY = int(*posX + eps);

  double dx = cos(*dirR);
  double dy = sin(*dirR);

  bool dirChanged = false;

  /* �߽��� */
  if (PosHasLeftEdge(gridX, gridY)) {
    // *posX = std::max(*posX, gridY + r);
    if (gridY + r > *posX) {
      *posX = gridY + r;
      if (dx < 0) {
        std::cerr << "GameGraph::BulletBoxFit InvX" << std::endl;
        dx = -dx; /* x �����ٶȷ��� */
      }
      dirChanged = true;
    }
  }
  if (PosHasRightEdge(gridX, gridY)) {
    // *posX = std::min(*posX, gridY + 1 - r);
    if (gridY + 1 - r < *posX) {
      *posX = gridY + 1 - r;
      if (dx > 0) {
        std::cerr << "GameGraph::BulletBoxFit InvX" << std::endl;
        dx = -dx; /* x �����ٶȷ��� */
      }
      dirChanged = true;
    }
  }
  if (PosHasTopEdge(gridX, gridY)) {
    // *posY = std::max(*posY, gridX + r);
    if (gridX + r > *posY) {
      *posY = gridX + r;
      if (dy < 0) {
        std::cerr << "GameGraph::BulletBoxFit InvY" << std::endl;
        dy = -dy; /* y �����ٶȷ��� */
      }
      dirChanged = true;
    }
  }
  if (PosHasBottomEdge(gridX, gridY)) {
    // *posY = std::min(*posY, gridX + 1 - r);
    if (gridX + 1 - r < *posY) {
      *posY = gridX + 1 - r;
      if (dy > 0) {
        std::cerr << "GameGraph::BulletBoxFit InvY" << std::endl;
        dy = -dy; /* y �����ٶȷ��� */
      }
      dirChanged = true;
    }
  }

  /* ������ */
  /*if (PosHasTopLeft(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY, gridX, r);
  }
  if (PosHasTopRight(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY + 1, gridX, r);
  }
  if (PosHasBottomLeft(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY, gridX + 1, r);
  }
  if (PosHasBottomRight(gridX, gridY)) {
    Utils::PointPush(posX, posY, gridY + 1, gridX + 1, r);
  }*/

  if (dirChanged) {
    *dirR = Utils::GetDirByDxDy(dx, dy);
  }
}

bool GameGraph::PosHasTopEdge(int gridX, int gridY) const {
  if (!InGraphGrid(gridX, gridY)) {
    return false;
  }
  if ((mGameGraph[gridX][gridY]).hasTop || !InGraphGrid(gridX - 1, gridY))
    return true;
  else
    return false;
}

bool GameGraph::PosHasBottomEdge(int gridX, int gridY) const {
  if (!InGraphGrid(gridX, gridY)) {
    return false;
  }
  if (!InGraphGrid(gridX + 1, gridY) || (mGameGraph[gridX + 1][gridY]).hasTop)
    return true;
  else
    return false;
}

bool GameGraph::PosHasLeftEdge(int gridX, int gridY) const {
  if (!InGraphGrid(gridX, gridY)) {
    return false;
  }
  if (!InGraphGrid(gridX, gridY - 1) || (mGameGraph[gridX][gridY]).hasLeft)
    return true;
  else
    return false;
}

bool GameGraph::PosHasRightEdge(int gridX, int gridY) const {
  if (!InGraphGrid(gridX, gridY)) {
    return false;
  }
  if (!InGraphGrid(gridX, gridY + 1) || (mGameGraph[gridX][gridY + 1]).hasLeft)
    return true;
  else
    return false;
}

bool GameGraph::PosHasTopLeft(int gridX, int gridY) const {
  return PosHasLeftEdge(gridX, gridY) || PosHasTopEdge(gridX, gridY) ||
         PosHasLeftEdge(gridX - 1, gridY) || PosHasTopEdge(gridX, gridY - 1);
}

bool GameGraph::PosHasTopRight(int gridX, int gridY) const {
  return PosHasLeftEdge(gridX, gridY + 1) || PosHasTopEdge(gridX, gridY + 1) ||
         PosHasLeftEdge(gridX - 1, gridY + 1) || PosHasTopEdge(gridX, gridY);
}

bool GameGraph::PosHasBottomLeft(int gridX, int gridY) const {
  return PosHasLeftEdge(gridX + 1, gridY) || PosHasTopEdge(gridX + 1, gridY) ||
         PosHasLeftEdge(gridX, gridY) || PosHasTopEdge(gridX + 1, gridY - 1);
}

bool GameGraph::PosHasBottomRight(int gridX, int gridY) const {
  return PosHasLeftEdge(gridX + 1, gridY + 1) || PosHasTopEdge(gridX + 1, gridY + 1) ||
         PosHasLeftEdge(gridX, gridY + 1) || PosHasTopEdge(gridX + 1, gridY);
}

int GameGraph::GetNodeId(int posX, int posY) const {
  assert(InGraphGrid(posX, posY));

  return posX * mWidth + posY + 1; /* �±�� 1 ��ʼ */
}

void GameGraph::GetNodePos(int nodeId, int& x, int& y) const {
  nodeId--;
  y = nodeId % mWidth;
  x = nodeId / mWidth;

  assert(InGraphGrid(x, y));
}

void GameGraphEdges::DebugShow() const {
  std::cerr << "(" << (int)hasLeft << ", " << (int)hasTop << ") ";
}
