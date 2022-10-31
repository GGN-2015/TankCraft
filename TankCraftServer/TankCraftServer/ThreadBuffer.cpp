#include <cassert>

#include "IMessage.h"
#include "ThreadBuffer.h"
#include "Utils.h"

ThreadBuffer::ThreadBuffer()
{
	mUserID = -1;
	mLastKillListTime = mLastShootTime = -1;
}

void ThreadBuffer::DumpMessage(IMessage* iMessage)
{
	mIMessageList.push_back(iMessage);
}

void ThreadBuffer::ClearDumpedMessage()
{
	for (auto iMessage : mIMessageList) {
		delete iMessage;
	}
	mIMessageList.clear();
}

void ThreadBuffer::GetTcpDataFromDumpedMessage(TcpData* pTcpDataGet)
{
	int totalLen = 4;
	for (auto pMsg : mIMessageList) {
		assert(pMsg->GetRawDataLength() % 2 == 0);

		totalLen += pMsg->GetRawDataLength(); /* 计算数据总长度 */
	}

	int pos = 0;
	unsigned short cnt = (unsigned short)mIMessageList.size(); // 消息总数
	char* buffer = new char[totalLen];

	/* 载入各个块的信息 */
	Utils::DumpUnsignedShortToBuffer(buffer, pos, cnt); pos += 2;
	for (auto pMsg : mIMessageList) {

		/* 获取二进制数据 */
		TcpData tmpTcpData;
		pMsg->GetRawData(&tmpTcpData);

		Utils::DumpTcpDataToBuffer(buffer, pos, &tmpTcpData);
		pos += tmpTcpData.GetLength();

		/* 二者给出的数据长度必须一致 */
		assert(tmpTcpData.GetLength() == pMsg->GetRawDataLength());
	}

	/* 载入总校系数 */
	char evenAns = 0, oddAns = 0;
	Utils::GetSanityInteger(buffer, pos, &evenAns, &oddAns); /* 校验系数不计算 pos 处 */
	buffer[pos + 0] = (char)evenAns;
	buffer[pos + 1] = (char)oddAns ; pos += 2;
	assert(pos == totalLen); /* 此时数据必须恰好填满了 */

	pTcpDataGet->SetData(buffer, totalLen);
	delete[] buffer;
}

void ThreadBuffer::SetUserID(int nID)
{
	mUserID = nID;
}

bool ThreadBuffer::InGame() const
{
	return mUserID != -1; /* UserId = -1 表示尚未登录，没有 UserInfo */
}
