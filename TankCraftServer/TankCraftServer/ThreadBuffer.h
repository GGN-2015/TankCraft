#pragma once
#include "IMessage.h"

/* 本类的实现不需要考虑线程安全，因为我们保证每个线程各自有一个 ThreadBuffer */
class ThreadBuffer
{
public:
	void DumpMessage(IMessage* iMessage); /* 向缓冲区中缓冲一条消息 */
	void ClearDumpedMessage();            /* 清空所有待发送的消息 */

	void GetTcpDataFromDumpedMessage(TcpData* pTcpDataGet); /* 获取要被发送的 TcpData */

private:
	IMessageList mIMessageList; /* 待发送的消息队列，服务端有义务在清空这个队列时对其中内容进行释放 */
};
