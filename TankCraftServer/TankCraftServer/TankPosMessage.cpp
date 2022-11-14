#include <cassert>

#include "TankPosMessage.h"
#include "TcpData.h"

TankPosMessage::TankPosMessage(const TcpData* mapData)
{
  mTcpData = TcpData::AllocTcpData(__FILE__, __LINE__);
  mTcpData->SetData(mapData->GetData(), mapData->GetLength());
}

TankPosMessage::~TankPosMessage()
{
	delete mTcpData;
}

void TankPosMessage::GetRawData(TcpData* tcpData)
{
	tcpData->SetData(mTcpData->GetData(), mTcpData->GetLength());
}

int TankPosMessage::GetRawDataLength()
{
	return mTcpData->GetLength();
}

void TankPosMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
	assert(false);
}

void TankPosMessage::DebugShow()
{
	mTcpData->DebugShow("[TankPosMessage] ");
}
