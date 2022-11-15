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
  SysUtils::SRand(); /* ÿ���̶߳���Ҫ������������� */

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

  /* ��ͻ��˷���̹��λ����Ϣ */
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

    totalLen += pMsg->GetRawDataLength(); /* ���������ܳ��� */
  }

  int pos = 0;
  unsigned short cnt = (unsigned short)mIMessageList.size();  // ��Ϣ����
  char* buffer = new char[totalLen];

  /* ������������Ϣ */
  Utils::DumpUnsignedShortToBuffer(buffer, pos, cnt);
  pos += 2;
  for (auto pMsg : mIMessageList) {
    /* ��ȡ���������� */
    // TcpData tmpTcpData;
    std::shared_ptr<TcpData> tmpTcpData(TcpData::AllocTcpData(__FILE__, __LINE__, false));
    pMsg->GetRawData(tmpTcpData.get());

    Utils::DumpTcpDataToBuffer(buffer, pos, tmpTcpData.get());
    pos += tmpTcpData->GetLength();

    /* ���߸��������ݳ��ȱ���һ�� */
    assert(tmpTcpData->GetLength() == pMsg->GetRawDataLength());
  }

  /* ������Уϵ�� */
  char evenAns = 0, oddAns = 0;
  Utils::GetSanityInteger(buffer, pos, &evenAns,
                          &oddAns); /* У��ϵ�������� pos �� */
  if (pos + 2 <= totalLen) {
    buffer[pos + 0] = (char)evenAns;
    buffer[pos + 1] = (char)oddAns;
    pos += 2;
  }
  assert(pos == totalLen); /* ��ʱ���ݱ���ǡ�������� */

  pTcpDataGet->SetData(buffer, totalLen);
  delete[] buffer;
}

void ThreadBuffer::SetUserID(int nID) { mUserID = nID; }

bool ThreadBuffer::InGame() const {
  return mUserID != -1; /* UserId = -1 ��ʾ��δ��¼��û�� UserInfo */
}

int ThreadBuffer::GetUserID() const { return mUserID; }

bool ThreadBuffer::HasGraphTcpDataCache() const {
  return mGraphTcpDataCache != nullptr;
}

void ThreadBuffer::DumpGraphTcpDataIntoMessageList(GameDatabase* Gdb) {
  /* ��ͼ�������� */
  if (!HasGraphTcpDataCache() || GraphTcpDataCacheOutofData(Gdb)) {
    FreeGraphTcpDataCache();

    Gdb->lock();

    mGraphTcpDataCache = TcpData::AllocTcpData(__FILE__, __LINE__, false);
    Gdb->GetGraphTcpData(mGraphTcpDataCache);

    Gdb->unlock();
  }

  /* ע�������Ϣ������ vector �������� */
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
