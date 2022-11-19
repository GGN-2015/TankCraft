#pragma once

#define BULLET_INVALID_TIME (0.1) /* 子弹在刚发射的 0.1s 内没有威力 */

class BulletInfo {
 public:
  double posX, posY, dirR, timeT;
  int userId;
  unsigned short bulletId;

  BulletInfo(double nPosX, double nPosY, double nDirR, double nTimeT,
             int nUserId);

  /* 申请一个新的子弹编号 */
  static unsigned short NewBulletId();

  bool TouchCircle(double uPosX, double uPosY, double bulletR, double tankR) const;
  bool Available() const; /* 检查子弹的有效性，刚发出的子弹是无效的 */
};
