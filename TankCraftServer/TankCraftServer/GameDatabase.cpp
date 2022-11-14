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
  pGameDatabasePhysicalEngineThread->join(); /* �ȴ� 30ms ֮����� */
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

  mUserInfoList.push_back(pUserInfo); /* ����һ���û� */
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
      userNameLenSum + userCnt * 12 + 2; /* �����ټ����û��������� 2 �ֽ� */
  int totalDataLen = 4 + subDataLen;

  char* buf = new char[totalDataLen];
  Utils::DumpUnsignedShortToBuffer(buf, 0, 3);          /* User Msg */
  Utils::DumpUnsignedShortToBuffer(buf, 2, subDataLen); /* �������ݳ��� */
  Utils::DumpUnsignedShortToBuffer(buf, 4, userCnt);    /* �û����� */

  int pos = 6;
  for (auto pUserInfo : mUserInfoList) {
    /* ��ȡһ���û������� */
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
  Utils::DumpUnsignedShortToBuffer(buf, 0, 4); /* 4 ����Ϣ��ʾ̹��λ�� */
  Utils::DumpUnsignedShortToBuffer(buf, 2, totalDataLen - 4); /* ���ݲ��ֳ��� */
  Utils::DumpUnsignedShortToBuffer(buf, 4,
                                   (int)mUserInfoList.size()); /* ̹���� */
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

  pTcpData->SetData(buf, totalDataLen); /* ��仰����д */
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
    int nUserId) { /* ע��������ܷ��ؿ�ָ�� */
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
    /* ��ȡ����̹��λ�� */
    TankPosMap tankPosMap; /* ID => TankPos ��ӳ�� */

    /* ��ȡ̹�˵İ������� */
    KeyStatusMap keyStatusMap;

    /* �ٽ�����ʼ */
    pGameDatabase
        ->lock(); /* ------------------------------------------------------- */

    /* ��ȡ̹��λ�� */
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->GetTankPos(&tankPosMap);
    }

    /* ����ʱ��� */
    double timeNow = Utils::GetClockTime();
    double dT =
        timeNow - pGameDatabase->GetLastFrameTime(); /* ������һ֡�ļ���ʱ�� */

    /* ��ȡ̹�˼���״̬ */
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->GetTankKeyStatus(&keyStatusMap);
    }

    /* ���ö��һ������Ȼ���߹�ȥ, ֱ���� tankPosMap ���޸� */
    for (auto& pTankPos : tankPosMap) {
      int tankUserId = pTankPos.first;
      TankPos oldTankPos = pTankPos.second; /* ����һ�� */

      double Dx = cos(oldTankPos.dirR) * TANK_SPEED * dT;
      double Dy = sin(oldTankPos.dirR) * TANK_SPEED * dT;
      double Dr = TANK_ROTATE_SPEED * dT;

      /* �����µ�̹��λ�� */
      TankPos newTankPos = oldTankPos;
      if ((keyStatusMap.find(tankUserId))->second.MoveForward()) {
        newTankPos.posX += Dx;
        newTankPos.posY += Dy;
      } else if ((keyStatusMap.find(tankUserId))->second.MoveBackward()) {
        newTankPos.posX -= Dx;
        newTankPos.posY -= Dy;
      }

      /* ������ת�¼� */
      if ((keyStatusMap.find(tankUserId))->second.TurnLeft()) {
        newTankPos.dirR -= Dr;
      } else if ((keyStatusMap.find(tankUserId))->second.TurnRight()) {
        newTankPos.dirR += Dr;
      }
      Utils::UnifyDirection(&newTankPos.dirR);

      /* ���Ӵ��� */
      pGameDatabase->mGameGraph.BoxFit(
          &newTankPos.posX,
          &newTankPos.posY, TANK_RADIUS + WALL_WIDTH); /* ��֤���ڸ��ӵ�����ǽ��Լ����ǰ̹��*/

      pTankPos.second.SetX(newTankPos.posX); /* ����̹�˵�ʵ��λ�� */
      pTankPos.second.SetY(newTankPos.posY);
      pTankPos.second.SetD(newTankPos.dirR);
    }

    /* �޸�����̹��λ�� */
    for (auto pUserInfo : pGameDatabase->mUserInfoList) {
      pUserInfo->SetTankPos(&tankPosMap);
    }
    pGameDatabase->SetLastFrameTime(timeNow);

    pGameDatabase
        ->unlock(); /* ---------------------------------------------------------
                     */
    /* �ٽ������� */

    /* ÿ 5ms �ػ�һ�� */
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

  /* ��������������Ⱦ�߳� */
  pGameDatabasePhysicalEngineThread =
      new std::thread(GameDatabasePhsicalEngineThreadFunction, this);
  pGameDatabasePhysicalEngineThread->detach();
}
