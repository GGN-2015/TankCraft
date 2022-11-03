#include <cassert>
#include <iostream>

#include "TcpData.h"
#include "UserInfoMessage.h"

UserInfoMessage::UserInfoMessage(const TcpData* mapData)
{
	mTcpData = new TcpData;
	mTcpData->SetData(mapData->GetData(), mapData->GetLength());
}

UserInfoMessage::~UserInfoMessage()
{
	delete mTcpData;
}

void UserInfoMessage::GetRawData(TcpData* tcpData)
{
	tcpData->SetData(mTcpData->GetData(), mTcpData->GetLength());
}

int UserInfoMessage::GetRawDataLength()
{
	return mTcpData->GetLength();
}

void UserInfoMessage::Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
{
	assert(false);
}

void UserInfoMessage::DebugShow()
{
	mTcpData->DebugShow("[UserInfoMessage] ");
}
