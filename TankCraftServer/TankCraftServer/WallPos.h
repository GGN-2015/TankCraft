#pragma once
#include <vector>

class WallPos;
typedef std::vector<WallPos> WallPosList;

struct PointPos { /* ���� */
  double posX;
  double posY;

  static PointPos Sub(PointPos lhs, PointPos rhs);    /* �������� */
  static double Dot(PointPos lhs, PointPos rhs);      /* ��˻� */
  static double Distance(PointPos lhs, PointPos rhs); /* ������� */
  static bool InCircle(PointPos nPointPos, double posX, double posY,
                       double r);               /* �жϵ��Ƿ���Բ�� */
  static PointPos Mul(PointPos nPos, double p); /* �������� */
  static PointPos Unit(PointPos nPos);          /* ��λ���� */
  double Len() const;
};

class WallPos {
 public:
  void SetFrom(PointPos nPos);
  void SetTo(PointPos nPos);

  /* ���ǽ�Ƿ��̹���ཻ */
  bool Crash(double posX, double posY, double r) const;
  void DebugOutput() const;

 private:
  double Distance(double posX, double posY) const; /* ����㵽���ߵľ��� */
  PointPos GetDirVec() const;                      /* ��ȡ�������� */

  PointPos mFromPos, mToPos;
};
