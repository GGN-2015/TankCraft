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
  /* ���߾��뵽��ľ������ r, һ�����ཻ */
  if (Distance(posX, posY) > r) {
    return false;
  }

  /* ��һ���˵���Բ��һ���ཻ */
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

  if (0 <= indexPos && indexPos <= 1) { /* ��� D �� FT֮�䣬��˵���ཻ */
    return true;
  } else {
    return false;
  }
}

PointPos PointPos::Sub(PointPos lhs, PointPos rhs) {
  /* �������� */
  PointPos ans = {};
  ans.posX = lhs.posX - rhs.posX;
  ans.posY = lhs.posY - rhs.posY;
  return ans;
}

double PointPos::Distance(PointPos lhs, PointPos rhs) {
  return Sub(lhs, rhs).Len();
}

double PointPos::Len() const {
  /* ������������ */
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
