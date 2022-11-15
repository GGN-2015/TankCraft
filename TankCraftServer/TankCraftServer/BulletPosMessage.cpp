#include <cassert>

#include "BulletPosMessage.h"
#include "TcpData.h"

BulletPosMessage::BulletPosMessage(const TcpData* mapData) {
  mTcpData = TcpData::AllocTcpData(__FILE__, __LINE__);
  mTcpData->SetData(mapData->GetData(), mapData->GetLength());
}

BulletPosMessage::~BulletPosMessage() { delete mTcpData; }

void BulletPosMessage::GetRawData(TcpData* tcpData) {
  tcpData->SetData(mTcpData->GetData(), mTcpData->GetLength());
}

int BulletPosMessage::GetRawDataLength() { return mTcpData->GetLength(); }

void BulletPosMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb) {
  assert(false);
}

void BulletPosMessage::DebugShow() { mTcpData->DebugShow("[BulletPosMessage] "); }
