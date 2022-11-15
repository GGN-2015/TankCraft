#include "ThreadBuffer.h"

#include <cassert>
#include <iostream>

#include "BulletPosMessage.h"
#include "GameDatabase.h"
#include "IMessage.h"
#include "MapMessage.h"
#include "SysUtils.h"
#include "TankPosMessage.h"
#include "Utils.h"

ThreadBuffer::ThreadBuffer() {
  SysUtils::SRand(); /* 每个线程都需要重置随机数种子 */

  mMaxSentId = mUserID = -1;
  mLastGetGraphTime = mLastKillListTime = mLastShootTime = -1;
}

ThreadBuffer::~ThreadBuffer() { FreeGraphTcpDataCache(); }

void ThreadBuffer::DumpMessage(IMessage* iMessage) {
  mIMessageList.push_back(iMessage);
}

void ThreadBuffer::DumpTankPosMessage(GameDatabase* Gdb) {
  // TcpData tmpTcpData;
  std::shared_ptr<TcpData> tmpTcpData(TcpData::AllocTcpData(__FILE__, __LINE__, false));
  Gdb->GetTankPosMessage(tmpTcpData.get());

  /* 向客户端反馈坦克位置信息 */
  // std::cerr << "[DumpTankPosMessage] tmpTcpData.GetLength() = "
  //          << tmpTcpData.GetLength() << std::endl;

  DumpMessage(new TankPosMessage(tmpTcpData.get()));
}

void ThreadBuffer::DumpBulletPosMessage(GameDatabase* Gdb) {
  std::shared_ptr<TcpData> tmpTcpData(
      TcpData::AllocTcpData(__FILE__, __LINE__, false));

  // std::cerr << "ThreadBuffer::DumpBulletPosMessage" << std::endl;
  Gdb->GetBulletPosMessage(tmpTcpData.get());

  // tmpTcpData->DebugShow("[BulletPosMessage] ");
  // system("pause");
  if (tmpTcpData->GetLength() > 6) {
    DumpMessage(new BulletPosMessage(tmpTcpData.get()));
  }
}

void ThreadBuffer::ClearDumpedMessage() {
  for (auto iMessage : mIMessageList) {
    delete iMessage;
  }
  mIMessageList.clear();
}

void ThreadBuffer::GetTcpDataFromDumpedMessage(TcpData* pTcpDataGet) {
  int totalLen = 4;
  for (auto pMsg : mIMessageList) {
    assert(pMsg->GetRawDataLength() % 2 == 0);

    totalLen += pMsg->GetRawDataLength(); /* 计算数据总长度 */
  }

  int pos = 0;
  unsigned short cnt = (unsigned short)mIMessageList.size();  // 消息总数
  char* buffer = new char[totalLen];

  /* 载入各个块的信息 */
  Utils::DumpUnsignedShortToBuffer(buffer, pos, cnt);
  pos += 2;
  for (auto pMsg : mIMessageList) {
    /* 获取二进制数据 */
    // TcpData tmpTcpData;
    std::shared_ptr<TcpData> tmpTcpData(TcpData::AllocTcpData(__FILE__, __LINE__, false));
    pMsg->GetRawData(tmpTcpData.get());

    Utils::DumpTcpDataToBuffer(buffer, pos, tmpTcpData.get());
    pos += tmpTcpData->GetLength();

    /* 二者给出的数据长度必须一致 */
    assert(tmpTcpData->GetLength() == pMsg->GetRawDataLength());
  }

  /* 载入总校系数 */
  char evenAns = 0, oddAns = 0;
  Utils::GetSanityInteger(buffer, pos, &evenAns,
                          &oddAns); /* 校验系数不计算 pos 处 */
  if (pos + 2 <= totalLen) {
    buffer[pos + 0] = (char)evenAns;
    buffer[pos + 1] = (char)oddAns;
    pos += 2;
  }
  assert(pos == totalLen); /* 此时数据必须恰好填满了 */

  pTcpDataGet->SetData(buffer, totalLen);
  delete[] buffer;
}

void ThreadBuffer::SetUserID(int nID) { mUserID = nID; }

bool ThreadBuffer::InGame() const {
  return mUserID != -1; /* UserId = -1 表示尚未登录，没有 UserInfo */
}

int ThreadBuffer::GetUserID() const { return mUserID; }

bool ThreadBuffer::HasGraphTcpDataCache() const {
  return mGraphTcpDataCache != nullptr;
}

void ThreadBuffer::DumpGraphTcpDataIntoMessageList(GameDatabase* Gdb) {
  /* 试图缓存数据 */
  if (!HasGraphTcpDataCache() || GraphTcpDataCacheOutofData(Gdb)) {
    FreeGraphTcpDataCache();

    Gdb->lock();

    mGraphTcpDataCache = TcpData::AllocTcpData(__FILE__, __LINE__, false);
    Gdb->GetGraphTcpData(mGraphTcpDataCache);

    Gdb->unlock();
  }

  /* 注：这个消息不允许被 vector 容器析构 */
  DumpMessage(new MapMessage(mGraphTcpDataCache));
}

void ThreadBuffer::SetGraphTimer() {
  mLastGetGraphTime = Utils::GetClockTime();
}

bool ThreadBuffer::GraphTcpDataCacheOutofData(GameDatabase* Gdb) const {
  double genTime = Gdb->GetLastGraphGenerateTime();
  return genTime > mLastGetGraphTime;
}

void ThreadBuffer::FreeGraphTcpDataCache() {
  delete mGraphTcpDataCache;
  mGraphTcpDataCache = nullptr;
}

void ThreadBuffer::SetMaxSentUserId(int nMaxSentId) { mMaxSentId = nMaxSentId; }

int ThreadBuffer::GetMaxSentUserId() const { return mMaxSentId; }
