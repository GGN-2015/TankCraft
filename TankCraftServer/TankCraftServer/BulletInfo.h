#pragma once

#define BULLET_INVALID_TIME (0.1) /* �ӵ��ڸշ���� 0.1s ��û������ */

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
  bool Available() const; /* ����ӵ�����Ч�ԣ��շ������ӵ�����Ч�� */
};
