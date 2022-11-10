#pragma once
#include <vector>

class WallPos;
typedef std::vector<WallPos> WallPosList;

struct PointPos { /* 向量 */
  double posX;
  double posY;

  static PointPos Sub(PointPos lhs, PointPos rhs);    /* 向量减法 */
  static double Dot(PointPos lhs, PointPos rhs);      /* 点乘积 */
  static double Distance(PointPos lhs, PointPos rhs); /* 计算距离 */
  static bool InCircle(PointPos nPointPos, double posX, double posY,
                       double r);               /* 判断点是否在圆内 */
  static PointPos Mul(PointPos nPos, double p); /* 数乘向量 */
  static PointPos Unit(PointPos nPos);          /* 单位向量 */
  double Len() const;
};

class WallPos {
 public:
  void SetFrom(PointPos nPos);
  void SetTo(PointPos nPos);

  /* 检测墙是否和坦克相交 */
  bool Crash(double posX, double posY, double r) const;
  void DebugOutput() const;

 private:
  double Distance(double posX, double posY) const; /* 计算点到基线的距离 */
  PointPos GetDirVec() const;                      /* 获取方向向量 */

  PointPos mFromPos, mToPos;
};
