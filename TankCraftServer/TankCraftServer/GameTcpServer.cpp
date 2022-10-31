#include <vector>
#include "DataParser.h"
#include "GameTcpServer.h"

GameTcpServer::GameTcpServer(const char* ip, int port): TcpServer(ip, port)
{
}

void GameTcpServer::GetTcpDataResult(const TcpData* pTcpDataRecv, TcpData* pTcpDataSend)
{
	/* ʹ�����ݽ���������Ϣ���н����ɶ��ֶ�������Ϣ */
	DataParser dataParser;
	IMessageList iClientRequestList;
	dataParser.Parse(pTcpDataRecv, &iClientRequestList, DataParser::GetGlobalClientRequestParserList());

}
