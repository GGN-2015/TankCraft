#include <vector>
#include "DataParser.h"
#include "GameTcpServer.h"

GameTcpServer::GameTcpServer(const char* ip, int port): TcpServer(ip, port)
{
}

void GameTcpServer::GetTcpDataResult(const TcpData* pTcpDataRecv, TcpData* pTcpDataSend)
{
	/* 使用数据解析器将消息序列解析成多种多样的消息 */
	DataParser dataParser;
	IMessageList iClientRequestList;
	dataParser.Parse(pTcpDataRecv, &iClientRequestList, DataParser::GetGlobalClientRequestParserList());

}
