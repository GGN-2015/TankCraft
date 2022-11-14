#include "GameDatabase.h"

#include <cassert>
#include <chrono>
#include <iostream>

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
    std::shared_ptr<TcpData> tmpTcpData(TcpData::AllocTcpData(__FILE__, __LINE__, false));
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
    /* 获取所有坦克位置 */
    TankPosMap tankPosMap; /* ID => TankPos 的映射 */

    /* 获取坦克的按键数据 */
    KeyStatusMap keyStatusMap;

    /* 临界区开始 */
    pGameDatabase
        ->lock(); /* ------------------------------------------------------- */

    /* 获取坦克位置 */
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->GetTankPos(&tankPosMap);
    }

    /* 计算时间差 */
    double timeNow = Utils::GetClockTime();
    double dT =
        timeNow - pGameDatabase->GetLastFrameTime(); /* 计算上一帧的计算时间 */

    /* 获取坦克键盘状态 */
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->GetTankKeyStatus(&keyStatusMap);
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
          &newTankPos.posX,
          &newTankPos.posY, TANK_RADIUS + WALL_WIDTH); /* 保证所在格子的四面墙能约束当前坦克*/

      pTankPos.second.SetX(newTankPos.posX); /* 设置坦克的实际位置 */
      pTankPos.second.SetY(newTankPos.posY);
      pTankPos.second.SetD(newTankPos.dirR);
    }

    /* 修改所有坦克位置 */
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->SetTankPos(&tankPosMap);
    }
    pGameDatabase->SetLastFrameTime(timeNow);

    pGameDatabase
        ->unlock(); /* ---------------------------------------------------------
                     */
    /* 临界区结束 */

    /* 每 5ms 重绘一次 */
    SysUtils::Sleep(GAME_DATABASE_PHISICAL_FRAME_PERIOD);
  }
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
