#include "GameDatabase.h"

#include <cassert>
#include <chrono>
#include <iostream>

#include "BulletInfo.h"
#include "GameGraph.h"
#include "SysUtils.h"
#include "UserInfo.h"
#include "Utils.h"

GameDatabase* GameDatabase::pGlobalGameDatabase = nullptr;

GameDatabase::~GameDatabase() {
  mGameDatabaseStatus = GAME_DATABASE_STOP;
  pGameDatabasePhysicalEngineThread->join(); /* 等待 30ms 之后结束 */
}

GameDatabase* GameDatabase::GetGlobalGameDatabase() {
  if (pGlobalGameDatabase == nullptr) {
    pGlobalGameDatabase = new GameDatabase;
  }
  return pGlobalGameDatabase;
}

bool GameDatabase::CheckUserNameExist(std::wstring userName) const {
  bool ans = false;
  for (auto pUserInfo : mUserInfoList) {
    if (pUserInfo->GetUserInfoName() == userName) {
      ans = true;
      break;
    }
  }
  return ans;
}

int GameDatabase::AllocNxtUserId() { return ++mUserIdNow; }

int GameDatabase::GetUserCount() const { return (int)mUserInfoList.size(); }

void GameDatabase::AddUser(int nUserId, std::wstring nUserName) {
  UserInfo* pUserInfo = new UserInfo(nUserId);
  pUserInfo->SetUserInfoName(nUserName);
  pUserInfo->SetUserColor(UserColor::GetRandomColor());
  pUserInfo->SetTankPosRandomly(mGameGraph.GetHeight(), mGameGraph.GetWidth());

  mUserInfoList.push_back(pUserInfo); /* 增加一个用户 */
#ifdef GAME_DATABASE_DEBUG
  std::wcerr << L"GameDatabase::AddUser() {\n    nUserId = " << nUserId
             << L"\n    nUserName = " << nUserName << "\n}" << std::endl;
#endif
}

void GameDatabase::DelUser(int nUserId) {
  int pos = -1;
  for (int i = 0; i < (int)mUserInfoList.size(); i += 1) {
    if (mUserInfoList[i]->GetUserId() == nUserId) {
      pos = i;
      break;
    }
  }

  assert(pos != -1);
  std::wcerr << L"[GameDatabase::DelUser] UserName = "
             << mUserInfoList[pos]->GetUserInfoName() << std::endl;
  mUserInfoList.erase(mUserInfoList.begin() + pos);
}

void GameDatabase::GenerateNewMap(int mHeight, int mWidth, double alpha) {
  mGameGraph.SetSize(mHeight, mWidth, alpha);
}

void GameDatabase::GetGraphTcpData(TcpData* pGraphTcpDataCache) const {
  mGameGraph.GetTcpData(pGraphTcpDataCache);
}

double GameDatabase::GetLastGraphGenerateTime() const {
  return mGameGraph.GetCreateTime();
}

void GameDatabase::GetTcpDataForUserInfoMessage(TcpData* nTcpData) {
  int userNameLenSum = 0;
  int userCnt = (int)mUserInfoList.size();

  for (auto pUserInfo : mUserInfoList) {
    userNameLenSum += 2 * (int)pUserInfo->GetUserInfoName().size();
  }

  int subDataLen =
      userNameLenSum + userCnt * 12 + 2; /* 这里少加了用户的总人数 2 字节 */
  int totalDataLen = 4 + subDataLen;

  char* buf = new char[totalDataLen];
  Utils::DumpUnsignedShortToBuffer(buf, 0, 3);          /* User Msg */
  Utils::DumpUnsignedShortToBuffer(buf, 2, subDataLen); /* 其后的数据长度 */
  Utils::DumpUnsignedShortToBuffer(buf, 4, userCnt);    /* 用户数量 */

  int pos = 6;
  for (auto pUserInfo : mUserInfoList) {
    /* 获取一个用户的数据 */
    std::shared_ptr<TcpData> tmpTcpData(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));
    pUserInfo->GetUserInfoTcpData(tmpTcpData.get());

    Utils::DumpTcpDataToBuffer(buf, pos, tmpTcpData.get());
    pos += tmpTcpData->GetLength();
  }

  assert(pos == totalDataLen);
  nTcpData->SetData(buf, totalDataLen);
  delete[] buf;
}

void GameDatabase::GetTankPosMessage(TcpData* pTcpData) const {
  int totalDataLen = 6 + 18 * (int)mUserInfoList.size();
  char* buf = new char[totalDataLen];
  Utils::DumpUnsignedShortToBuffer(buf, 0, 4); /* 4 号消息表示坦克位置 */
  Utils::DumpUnsignedShortToBuffer(buf, 2, totalDataLen - 4); /* 数据部分长度 */
  Utils::DumpUnsignedShortToBuffer(buf, 4,
                                   (int)mUserInfoList.size()); /* 坦克数 */
  int pos = 6;
  for (auto pUserInfo : mUserInfoList) {
    std::shared_ptr<TcpData> tmpTcpData(
        TcpData::AllocTcpData(__FILE__, __LINE__, false));

    pUserInfo->GetTankPosTcpData(tmpTcpData.get());
    Utils::DumpTcpDataToBuffer(buf, pos, tmpTcpData.get());

    pos += tmpTcpData->GetLength();
    assert(tmpTcpData->GetLength() == 18);
  }
  assert(pos == totalDataLen);

  pTcpData->SetData(buf, totalDataLen); /* 这句话总忘写 */
  delete[] buf;
}

void GameDatabase::GetBulletPosMessage(TcpData* pTcpData) const {
  int bulletCnt = (int)mBulletInfoList.size();
  int restLen = 2 + 12 * bulletCnt;
  int totalLen = 4 + restLen;

  char* buf = new char[totalLen];
  int pos = 0;

  Utils::DumpUnsignedShortToBuffer(buf, pos, 7);
  pos += 2; /* 七号消息表示消息的长度 */

  Utils::DumpUnsignedShortToBuffer(buf, pos, *(unsigned int*)&restLen);
  pos += 2; /* 剩余部分的消息长度 */

  Utils::DumpUnsignedShortToBuffer(buf, pos, *(unsigned int*)&bulletCnt);
  pos += 2; /* 子弹总数 */

  for (int i = 0; i < bulletCnt; i += 1) {
    float posX = (float)mBulletInfoList[i].posX;
    float posY = (float)mBulletInfoList[i].posY;

    Utils::DumpUnsignedShortToBuffer(buf, pos, 0); /* 炮弹是 0 类型实体 */
    pos += 2;

    Utils::DumpFloatToBuffer(buf, pos, posX);
    pos += 4; /* 横坐标 4 个字节 */

    Utils::DumpFloatToBuffer(buf, pos, posY);
    pos += 4; /* 纵坐标 4 个字节 */

    Utils::DumpUnsignedShortToBuffer(buf, pos, mBulletInfoList[i].bulletId);
    pos += 2; /* 实体 ID 2 个字节 */
  }

  pTcpData->SetData(buf, totalLen); /* 拷贝数据 */
  delete[] buf;
}

int GameDatabase::GetMaxUserId() const { return mUserIdNow; }

int GameDatabase::GetGameDatabaseStatusAtomic() const {
  lock();
  int ans = mGameDatabaseStatus;
  unlock();

  return ans;
}

UserInfo* GameDatabase::GetUserInfoByUserId(
    int nUserId) { /* 注意这里可能返回空指针 */
  for (auto pUserInfo : mUserInfoList) {
    if (pUserInfo->GetUserId() == nUserId) {
      return pUserInfo;
    }
  }
  return nullptr;
}

void GameDatabase::GameDatabasePhsicalEngineThreadFunction(
    GameDatabase* pGameDatabase) {
  while (pGameDatabase->GetGameDatabaseStatusAtomic() == GAME_DATABASE_RUN) {
    /* 临界区开始 */
    pGameDatabase
        ->lock(); /* ------------------------------------------------------- */

    GameDatabasePhsicalEngineTankFunction(
        pGameDatabase); /* 处理坦克的移动事件 */
    GameDatabasePhsicalEngineBulletFunction(
        pGameDatabase); /* 处理炮弹移动事件 */

    /* 设置上次绘制时间 */
    pGameDatabase->SetLastFrameTime(Utils::GetClockTime());

    pGameDatabase
        ->unlock(); /* ---------------------------------------------------- */
    /* 临界区结束 */

    /* 每 5ms 重绘一次 */
    SysUtils::Sleep(GAME_DATABASE_PHISICAL_FRAME_PERIOD);
  }
}

void GameDatabase::GameDatabasePhsicalEngineTankFunction(
    GameDatabase* pGameDatabase) {
  /* 获取所有坦克位置 */
  TankPosMap tankPosMap; /* ID => TankPos 的映射 */

  /* 获取坦克的按键数据 */
  KeyStatusMap keyStatusMap;

  /* 获取坦克位置 */
  for (auto pUserInfo : pGameDatabase->mUserInfoList) {
    pUserInfo->GetTankPos(&tankPosMap);
  }

  /* 计算时间差 */
  double timeNow = Utils::GetClockTime();
  double dT =
      timeNow - pGameDatabase->GetLastFrameTime(); /* 计算上一帧的计算时间 */

  IntSet canShootUserId; /* 记录哪些用户可以发射炮弹 */
  IntSet shootUserIdSet; /* 记录哪些用户发炮了 */

  /* 获取坦克键盘状态 */
  for (auto pUserInfo : pGameDatabase->mUserInfoList) {
    pUserInfo->GetTankKeyStatus(&keyStatusMap);
    if (pUserInfo->CanShoot()) {
      canShootUserId.insert(pUserInfo->GetUserId());
    }
  }

  /* 随机枚举一个方向然后走过去, 直接在 tankPosMap 上修改 */
  for (auto& pTankPos : tankPosMap) {
    int tankUserId = pTankPos.first;
    TankPos oldTankPos = pTankPos.second; /* 拷贝一个 */

    double Dx = cos(oldTankPos.dirR) * TANK_SPEED * dT;
    double Dy = sin(oldTankPos.dirR) * TANK_SPEED * dT;
    double Dr = TANK_ROTATE_SPEED * dT;

    /* 计算新的坦克位置 */
    TankPos newTankPos = oldTankPos;
    if ((keyStatusMap.find(tankUserId))->second.MoveForward()) {
      newTankPos.posX += Dx;
      newTankPos.posY += Dy;
    } else if ((keyStatusMap.find(tankUserId))->second.MoveBackward()) {
      newTankPos.posX -= Dx;
      newTankPos.posY -= Dy;
    }

    /* 处理旋转事件 */
    if ((keyStatusMap.find(tankUserId))->second.TurnLeft()) {
      newTankPos.dirR -= Dr;
    } else if ((keyStatusMap.find(tankUserId))->second.TurnRight()) {
      newTankPos.dirR += Dr;
    }
    Utils::UnifyDirection(&newTankPos.dirR);

    /* 盒子处理 */
    pGameDatabase->mGameGraph.BoxFit(
        &newTankPos.posX, &newTankPos.posY,
        TANK_RADIUS + WALL_WIDTH); /* 保证所在格子的四面墙能约束当前坦克*/

    /* 发射炮弹 */
    if ((keyStatusMap.find(tankUserId))->second.shoot == TANK_KEY_DOWN) {
      /* 无论是否能发炮，都帮他把按键抬起来 */

      pGameDatabase->SetKeyStatusForUser(tankUserId, TANK_SHOOT, TANK_KEY_UP);
      if (canShootUserId.count(tankUserId) > 0) { /* 可以发炮 */

        double eps = 1e-4;
        pGameDatabase->AddBullet(
            newTankPos.posX, newTankPos.posY, newTankPos.dirR,
            TANK_RADIUS + TANK_BULLET_RADIUS + eps, tankUserId);

        shootUserIdSet.insert(tankUserId);
      }
    }

    pTankPos.second.SetX(newTankPos.posX); /* 设置坦克的实际位置 */
    pTankPos.second.SetY(newTankPos.posY);
    pTankPos.second.SetD(newTankPos.dirR);
  }

  /* 修改所有坦克位置 */
  /* 修改所有坦克的发射状态信息 */
  for (auto pUserInfo : pGameDatabase->mUserInfoList) {
    pUserInfo->SetTankPos(&tankPosMap);

    if (shootUserIdSet.count(pUserInfo->GetUserId()) > 0) {
      pUserInfo->Shoot();
    }
  }
}

void GameDatabase::GameDatabasePhsicalEngineBulletFunction(
    GameDatabase* pGameDatabase) {
  double timeNow = Utils::GetClockTime();
  double dt = timeNow - pGameDatabase->GetLastFrameTime();

  std::set<int> nExpiredBulletId; /* 记录所有过期子弹的地址 */

  for (int i = 0; i < (int)pGameDatabase->mBulletInfoList.size(); i += 1) {
    BulletInfo& bulletPos = pGameDatabase->mBulletInfoList[i];

    if (timeNow - bulletPos.timeT >= TANK_BULLET_EXPIRED_TIME) {
      nExpiredBulletId.insert(i);
    } else {
      /* 子弹前进, 直接在原数组上修改 */
      bulletPos.posX += dt * cos(bulletPos.dirR) * BULLET_SPEED;
      bulletPos.posY += dt * sin(bulletPos.dirR) * BULLET_SPEED;

      pGameDatabase->mGameGraph.BulletBoxFit(
          &bulletPos.posX, &bulletPos.posY, /* 子弹的位置 */
          TANK_BULLET_RADIUS + WALL_WIDTH, &bulletPos.dirR
      );
    }
  }
  
  IntMap userIdBulletExpired; /* 所有子弹过期的用户 */

  /* 删除所有过期的子弹 */
  if (nExpiredBulletId.size() >= 1) {
    auto p = nExpiredBulletId.end();
    do {
      p--;

      int userId = pGameDatabase->mBulletInfoList[*p].userId;
      userIdBulletExpired[userId] += 1;

      pGameDatabase->mBulletInfoList.erase(
          pGameDatabase->mBulletInfoList.begin()  + * p);
    } while (p != nExpiredBulletId.begin());
  }

  /* 用户子弹超时处理 */
  pGameDatabase->UserBulletExpired(&userIdBulletExpired);
}

double GameDatabase::GetLastFrameTime() const { return mLastFrameTime; }

void GameDatabase::SetLastFrameTime(double nFrameTime) {
  mLastFrameTime = nFrameTime;
}

void GameDatabase::SetKeyStatusForUser(int nUserId, int nKeyId, bool status) {
  UserInfo* pUserInfo = GetUserInfoByUserId(nUserId);
  if (pUserInfo != nullptr) {
    std::cerr << "[GameDatabase::SetKeyStatusForUser]" << nUserId << ", "
              << nKeyId << ", " << status << std::endl;
    pUserInfo->GetKeyStatusObject()->GetStatusById(nKeyId) = status;
  } else {
    assert(false);
  }
}

void GameDatabase::AddBullet(double posX, double posY, double dirR, double disD,
                             int userId) {

  double dx = cos(dirR) * disD, dy = sin(dirR) * disD;
  posX += dx;
  posY += dy; /* 在坦克的前方 disD 放置子弹 */

  mBulletInfoList.push_back(BulletInfo(posX, posY, dirR, Utils::GetClockTime(), userId));
  std::cerr << "GameDatabase::AddBullet, bulletCnt = " << mBulletInfoList.size()
            << std::endl;
}

void GameDatabase::GetCanShootUserIdSet(IntSet* userIdSet) const {
  userIdSet->clear();
  for (auto userInfo : mUserInfoList) {
    if (userInfo->CanShoot()) {
      userIdSet->insert(userInfo->GetUserId());
    }
  }
}

void GameDatabase::UserBulletExpired(IntMap* userIdMapToBulletCnt) { 
  for (auto pUserInfo : mUserInfoList) {
    int userId = pUserInfo->GetUserId();
    if (userIdMapToBulletCnt->count(userId)) {
      pUserInfo->BulletExpired((*userIdMapToBulletCnt)[userId]);
    }
  }
}

GameDatabase::GameDatabase() {
  mUserIdNow = 0;
  mLastFrameTime = Utils::GetClockTime();

  mGameDatabaseStatus = GAME_DATABASE_RUN;
  mGameGraph.SetSize(8, 8, 0.3);

  /* 启动物理引擎渲染线程 */
  pGameDatabasePhysicalEngineThread =
      new std::thread(GameDatabasePhsicalEngineThreadFunction, this);
  pGameDatabasePhysicalEngineThread->detach();
}
