#include <vector>
#include "DataParser.h"
#include "GameTcpServer.h"
#include "TcpData.h"
#include "ThreadBuffer.h"

GameTcpServer::GameTcpServer(const char* ip, int port): TcpServer(ip, port)
{
}

void GameTcpServer::GetTcpDataResult(const TcpData* pTcpDataRecv, 
	TcpData* pTcpDataSend, ThreadBuffer* tb, GameDatabase* Gdb)
{
	/* 使用数据解析器将消息序列解析成多种多样的消息 */
	DataParser dataParser;
	IMessageList iClientRequestList;
	dataParser.Parse(pTcpDataRecv, &iClientRequestList, DataParser::GetGlobalClientRequestParserList());

	/* 根据消息内容做出改变，或者派发消息 */
	for (auto iClientRequest : iClientRequestList) {
		iClientRequest->Dispatch(tb, Gdb);
	}

	/* 完成工作后要记得释放收到的 Request 数据 */
	for (auto iClientRequest : iClientRequestList) {
		delete iClientRequest;
	}

	tb->DumpTankPosMessage(Gdb);                   /* 获取并发送坦克位置信息 */
	tb->GetTcpDataFromDumpedMessage(pTcpDataSend); /* 获取要被发送的消息 */
	tb->ClearDumpedMessage();                      /* 清空缓存的要发送的消息 */
}
