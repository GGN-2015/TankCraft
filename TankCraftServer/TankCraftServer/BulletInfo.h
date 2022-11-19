#pragma once
class BulletInfo {
 public:
  double posX, posY, dirR, timeT;
  int userId;
  unsigned short bulletId;

  BulletInfo(double nPosX, double nPosY, double nDirR, double nTimeT,
             int nUserId);

  /* ����һ���µ��ӵ���� */
  static unsigned short NewBulletId();
  bool TouchCircle(double uPosX, double uPosY, double bulletR, double tankR) const;
};
