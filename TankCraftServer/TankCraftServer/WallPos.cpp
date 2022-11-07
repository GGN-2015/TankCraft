#include "WallPos.h"

#include <algorithm>

void WallPos::SetFrom(PointPos nPos) { mFromPos = nPos; }

void WallPos::SetTo(PointPos nPos) { mToPos = nPos; }

bool PointPos::InCircle(PointPos nPointPos, double posX, double posY,
                        double r) {
  return PointPos::Distance({posX, posY}, nPointPos) <= r;
}

PointPos PointPos::Mul(PointPos nPos, double p) {
  nPos.posX *= p;
  nPos.posY *= p;
  return nPos;
}

PointPos PointPos::Unit(PointPos nPos) {
  double len = nPos.Len();

  nPos.posX /= len;
  nPos.posY /= len;
  return nPos;
}

bool WallPos::Crash(double posX, double posY, double r) const {
  /* 基线距离到点的距离大于 r, 一定不相交 */
  if (Distance(posX, posY) > r) {
    return false;
  }

  /* 有一个端点在圆里一定相交 */
  if (PointPos::InCircle(mFromPos, posX, posY, r) ||
      PointPos::InCircle(mToPos, posX, posY, r)) {
    return true;
  }

  const PointPos& F = mFromPos;
  const PointPos& T = mToPos;
  const PointPos& C = {posX, posY};

  const PointPos& vec = GetDirVec();

  double projectLen = PointPos::Dot(vec, PointPos::Sub(C, F)) / vec.Len();
  PointPos D = PointPos::Mul(PointPos::Unit(vec), projectLen);

  PointPos vecD = PointPos::Sub(D, F);
  double indexPos = PointPos::Dot(vecD, vec) / vec.Len();

  if (0 <= indexPos && indexPos <= 1) { /* 如果 D 在 FT之间，则说明相交 */
    return true;
  } else {
    return false;
  }
}

PointPos PointPos::Sub(PointPos lhs, PointPos rhs) {
  /* 向量做差 */
  PointPos ans = {};
  ans.posX = lhs.posX - rhs.posX;
  ans.posY = lhs.posY - rhs.posY;
  return ans;
}

double PointPos::Distance(PointPos lhs, PointPos rhs) {
  return Sub(lhs, rhs).Len();
}

double PointPos::Len() const {
  /* 计算向量长度 */
  return sqrt(posX * posX + posY * posY);
}

double PointPos::Dot(PointPos lhs, PointPos rhs) {
  return lhs.posX * rhs.posX + lhs.posY * rhs.posY;
}

double WallPos::Distance(double posX, double posY) const {
  PointPos vecV = PointPos::Sub({posX, posY}, mFromPos);
  PointPos vecU = GetDirVec();

  double theta = PointPos::Dot(vecU, vecV) / (vecU.Len() * vecV.Len());
  return abs(vecV.Len() * cos(theta));
}

PointPos WallPos::GetDirVec() const { return PointPos::Sub(mToPos, mFromPos); }
