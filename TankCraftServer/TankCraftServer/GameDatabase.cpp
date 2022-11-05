#include "GameDatabase.h"

#include <windows.h>

#include <cassert>
#include <chrono>
#include <iostream>

#include "GameGraph.h"
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
    TcpData tmpTcpData;
    pUserInfo->GetUserInfoTcpData(&tmpTcpData);

    Utils::DumpTcpDataToBuffer(buf, pos, &tmpTcpData);
    pos += tmpTcpData.GetLength();
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
    TcpData tmpTcpData;
    pUserInfo->GetTankPosTcpData(&tmpTcpData);
    Utils::DumpTcpDataToBuffer(buf, pos, &tmpTcpData);

    pos += tmpTcpData.GetLength();
    assert(tmpTcpData.GetLength() == 18);
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

void GameDatabase::GameDatabasePhsicalEngineThreadFunction(
    GameDatabase* pGameDatabase) {
  while (pGameDatabase->GetGameDatabaseStatusAtomic() == GAME_DATABASE_RUN) {
    /* 获取所有坦克位置 */
    TankPosMap tankPosMap; /* ID => TankPos 的映射 */
    pGameDatabase->lock();
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->GetTankPos(&tankPosMap);
    }
    pGameDatabase->unlock();

    /* 计算时间差 */
    double timeNow = Utils::GetClockTime();
    double dT =
        timeNow - pGameDatabase->GetLastFrameTime(); /* 计算上一帧的计算时间 */

    // std::cerr << "[PhsicalEngineThreadFunction] dT = " << dT << std::endl;

    /* 随机枚举一个方向然后走过去, 直接在 tankPosMap 上修改 */
    for (auto& pTankPos : tankPosMap) {
      TankPos oldTankPos = pTankPos.second; /* 拷贝一个 */

      double direction = Utils::GetRandomDouble() * Utils::Get2PI();
      pTankPos.second.dirR = direction;
      pTankPos.second.posX += TANK_SPEED * dT * cos(direction);
      pTankPos.second.posY += TANK_SPEED * dT * sin(direction);

      /* 如果新的坦克位置不合法那就不走 */
      if (!pGameDatabase->mGameGraph.InGraph(
              pTankPos.second.posX, pTankPos.second.posY, TANK_RADIUS)) {
        pTankPos.second = oldTankPos; /* 还原先前的位置 */
      }
    }

    /* 修改所有坦克位置 */
    pGameDatabase->lock();
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->SetTankPos(&tankPosMap);
    }
    pGameDatabase->SetLastFrameTime(timeNow);
    pGameDatabase->unlock();

    /* 每 30ms 重绘一次 */
    Sleep(GAME_DATABASE_PHISICAL_FRAME_PERIOD);
  }
}

double GameDatabase::GetLastFrameTime() const { return mLastFrameTime; }

void GameDatabase::SetLastFrameTime(double nFrameTime) {
  mLastFrameTime = nFrameTime;
}

GameDatabase::GameDatabase() {
  mUserIdNow = 0;
  mLastFrameTime = Utils::GetClockTime();

  mGameDatabaseStatus = GAME_DATABASE_RUN;
  mGameGraph.SetSize(16, 16, 0.4);

  /* 启动物理引擎渲染线程 */
  pGameDatabasePhysicalEngineThread =
      new std::thread(GameDatabasePhsicalEngineThreadFunction, this);
  pGameDatabasePhysicalEngineThread->detach();
}
