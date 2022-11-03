#include <cassert>

#include "MapMessage.h"
#include "TcpData.h"

MapMessage::MapMessage(const TcpData* mapData)
{
	mTcpData = new TcpData;
	mTcpData->SetData(mapData->GetData(), mapData->GetLength());
}

MapMessage::~MapMessage()
{
	delete mTcpData;
}

void MapMessage::GetRawData(TcpData* tcpData)
{
	tcpData->SetData(mTcpData->GetData(), mTcpData->GetLength());
}

int MapMessage::GetRawDataLength()
{
	return mTcpData->GetLength();
}

void MapMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
	assert(false);
}

void MapMessage::DebugShow()
{
	mTcpData->DebugShow("[MapMessage] ");
}
