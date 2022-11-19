#include "UserInfo.h"

#include <cassert>
#include <iostream>

#include "TcpData.h"
#include "Utils.h"

const double inf = 1e300;

UserInfo::UserInfo(int nUserId) {
  mUserId = nUserId;
  mKillCnt = 0;
  mLastKilledTime = -inf;
}

int UserInfo::GetUserId() const { return mUserId; }

std::wstring UserInfo::GetUserInfoName() const { return mUserName; }

void UserInfo::SetUserInfoName(std::wstring nUserName) {
  mUserName = nUserName;
}

void UserInfo::IncKillCnt(int incVal) {
  mKillCnt += incVal;

  /* 输出击杀数信息 */
  std::cerr << "[UserInfo::IncKillCnt] userId = " << mUserId
            << " mKillCnt = " << mKillCnt << std::endl;
}

void UserInfo::SetUserColor(unsigned char R, unsigned char G, unsigned char B,
                            unsigned char A) {
  mUserColor.R = R;
  mUserColor.G = G;
  mUserColor.B = B;
  mUserColor.A = A;
}

void UserInfo::SetUserColor(UserColor uc) { mUserColor = uc; }

void UserInfo::Killed(int nHeight, int nWidth) { 
  mLastKilledTime = Utils::GetClockTime();
  mTankPos.RandomPosition(nHeight, nWidth);
}

void UserInfo::GetUserInfoTcpData(TcpData* tcpData) const {
  /* 用户 ID, 登录名长度, 字符串, RGBA, 击杀数 */
  int dataLength = 4 + 2 + (2 * (int)mUserName.size()) + 4 + 2;
  int userNameLen = (2 * (int)mUserName.size());

  char* buf = new char[dataLength];
  Utils::DumpIntToBuffer(buf, 0, mUserId); /* pos0: 用户 ID 4 字节 */
  Utils::DumpUnsignedShortToBuffer(
      buf, 4, (unsigned short)userNameLen); /* pos4: 登录名长度 2 字节 */
  Utils::DumpRawStringToBuffer(buf, 6, (char*)mUserName.c_str(),
                               userNameLen); /* pos6: 字符串 */

  int pos = 6 + userNameLen;
  buf[pos++] = *(char*)&mUserColor.R; /* 坦克颜色 */
  buf[pos++] = *(char*)&mUserColor.G;
  buf[pos++] = *(char*)&mUserColor.B;
  buf[pos++] = *(char*)&mUserColor.A;

  Utils::DumpUnsignedShortToBuffer(buf, pos, mKillCnt); /* 击杀数 */
  pos += 2;
  assert(pos == dataLength);

  tcpData->SetData(buf, dataLength);
  delete[] buf;
}

void UserInfo::GetTankPos(TankPosMap* nTankPosMap) /* 将坦克未知存入 Map */
{
  (*nTankPosMap)[GetUserId()] = mTankPos;
}

void UserInfo::GetTankPos(double& posX, double& posY) const {
  posX = mTankPos.posX;
  posY = mTankPos.posY;
}

void UserInfo::GetTankKeyStatus(KeyStatusMap* nKeyStatusMap) {
  (*nKeyStatusMap)[GetUserId()] = mKeyStatus;
}

void UserInfo::SetTankPos(const TankPosMap* nTankPosMap) {
  auto pTankMsg = (*nTankPosMap).find(GetUserId());

  if (pTankMsg != nTankPosMap->end()) { /* 如果表中没找到则说明可能不变 */
    mTankPos = pTankMsg->second;
  }
}

void UserInfo::SetTankPosRandomly(int mHeight, int mWidth) {
  mTankPos.RandomPosition(mHeight, mWidth);
}

/* 坦克状态: 0:无, 1:霸体, 2:激光武器 */
int UserInfo::GetTankStatus() const {
  return ((int)mHasLaserWeapon << 1) | (int)CheckSuperArmor();
}

bool UserInfo::CheckSuperArmor() const {
  double timeNow = Utils::GetClockTime();
  if (timeNow < mLastKilledTime) {
    return false;
  } else {
    return timeNow - mLastKilledTime <= SUPER_ARMOR_TIME;
  }
}

void UserInfo::GetTankPosTcpData(TcpData* pTcpData) const {
  char buf[18] = {};
  Utils::DumpIntToBuffer((char*)buf, 0, GetUserId());
  Utils::DumpFloatToBuffer((char*)buf, 4, (float)mTankPos.posX);
  Utils::DumpFloatToBuffer((char*)buf, 8, (float)mTankPos.posY);
  Utils::DumpFloatToBuffer((char*)buf, 12, (float)mTankPos.dirR);
  Utils::DumpUnsignedShortToBuffer((char*)buf, 16, GetTankStatus());

  pTcpData->SetData(buf, 18);
}

KeyStatus* UserInfo::GetKeyStatusObject() { return &mKeyStatus; }

bool UserInfo::CanShoot() const {
  return mBulletCount < USER_BULLET_MAX &&
         Utils::GetClockTime() - mLastShootTime >= TANK_SHOOT_TIME_PERIOD;
}

void UserInfo::Shoot() {
  mBulletCount += 1;
  mLastShootTime = Utils::GetClockTime();
}

void UserInfo::BulletExpired(int bulletCnt) {
  mBulletCount -= bulletCnt;
  // mBulletCount = std::max(mBulletCount, 0);
  assert(mBulletCount >= 0);
}

UserColor::UserColor(unsigned char nR, unsigned char nG, unsigned char nB,
                     unsigned char nA) {
  R = nR;
  G = nG;
  B = nB;
  A = nA;
}

UserColor UserColor::GetRandomColor() {
  unsigned char ucList[] = {0, 0, 0};
  for (int i = 0; i <= 2; i += 1) {
    ucList[i] = Utils::GetRandUnsignedChar();
  }
  return UserColor(ucList[0], ucList[1], ucList[2], 255);
}

TankPos::TankPos(const TankPos& nTankPos) {
  posX = nTankPos.posX;
  posY = nTankPos.posY;
  dirR = nTankPos.dirR;
}

void TankPos::RandomPosition(int mHeight, int mWidth) {
  posX = (Utils::GetRandLongLong() % mWidth) + 0.5; /* 随机位置 */
  posY = (Utils::GetRandLongLong() % mHeight) + 0.5;
  dirR = Utils::GetRandomDouble() * Utils::Get2PI(); /* 随机方向 */
}

void TankPos::SetPosByAnotherTankPos(const TankPos* pTankPos) {
  if (pTankPos != nullptr) {
    posX = pTankPos->posX;
    posY = pTankPos->posY;
  }
}

void TankPos::SetX(double nPosX) { posX = nPosX; }

void TankPos::SetY(double nPosY) { posY = nPosY; }

void TankPos::SetD(double nDirD) { dirR = nDirD; }

KeyStatus::KeyStatus() { /* 所有键都没按下 */
  shoot = TANK_KEY_UP;
  turnRight = TANK_KEY_UP;
  forward = TANK_KEY_UP;
  turnLeft = TANK_KEY_UP;
  backward = TANK_KEY_UP;
}

static bool ErrorKeyStatus = TANK_KEY_UP;
bool& KeyStatus::GetStatusById(int mStatusId) {
  assert(0 <= mStatusId && mStatusId <= 4);

  switch (mStatusId) {
    case TANK_SHOOT:
      return shoot;
    case 1:
      return turnRight;
    case 2:
      return forward;
    case 3:
      return turnLeft;
    case 4:
      return backward;
  }

  assert(false);
  return ErrorKeyStatus;
}

bool KeyStatus::GetStatusById(int mStatusId) const {
  assert(0 <= mStatusId && mStatusId <= 4);

  switch (mStatusId) {
    case TANK_SHOOT:
      return shoot;
    case 1:
      return turnRight;
    case 2:
      return forward;
    case 3:
      return turnLeft;
    case 4:
      return backward;
  }
  return false;
}

bool KeyStatus::TurnLeft() const {
  return turnLeft == TANK_KEY_DOWN && turnRight == TANK_KEY_UP;
}

bool KeyStatus::TurnRight() const {
  return turnRight == TANK_KEY_DOWN && turnLeft == TANK_KEY_UP;
}

bool KeyStatus::MoveForward() const {
  return forward == TANK_KEY_DOWN && backward == TANK_KEY_UP;
}

bool KeyStatus::MoveBackward() const {
  return backward == TANK_KEY_DOWN && forward == TANK_KEY_UP;
}
