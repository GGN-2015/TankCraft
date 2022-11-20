#include "BulletInfo.h"

#include <iostream>

#include "Utils.h"

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

bool BulletInfo::TouchCircle(double uPosX, double uPosY, double bulletR,
                             double tankR) const {

  return Utils::PointDistance(uPosX, uPosY, posX, posY) <= bulletR + tankR;
}

bool BulletInfo::Available() const {
  bool ans = Utils::GetClockTime() - timeT >= BULLET_INVALID_TIME;
  // std::cerr << "BulletInfo::Available() ans = " << ans << std::endl;
  return ans;
}
