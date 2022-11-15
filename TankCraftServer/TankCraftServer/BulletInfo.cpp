#include "BulletInfo.h"

BulletInfo::BulletInfo(double nPosX, double nPosY, double nDirR, double nTimeT,
                       int nUserId) {
  posX = nPosX;
  posY = nPosY;
  dirR = nDirR;
  timeT = nTimeT;
  userId = nUserId;

  bulletId = BulletInfo::NewBulletId();
}

unsigned short BulletInfo::NewBulletId() {
  static unsigned short nowId = 0;
  if (nowId == 65535) {
    nowId = 0;
  } else {
    nowId += 1;
  }
  return nowId;
}
