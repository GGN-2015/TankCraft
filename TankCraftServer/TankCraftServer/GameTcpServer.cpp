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
#ifdef GAME_TCP_SERVER_DEBUG
	pTcpDataRecv->DebugShow("[Client] ");
#endif

	/* ʹ�����ݽ���������Ϣ���н����ɶ��ֶ�������Ϣ */
	DataParser dataParser;
	IMessageList iClientRequestList;
	dataParser.Parse(pTcpDataRecv, &iClientRequestList, DataParser::GetGlobalClientRequestParserList());

	/* ������Ϣ���������ı䣬�����ɷ���Ϣ */
	for (auto iClientRequest : iClientRequestList) {
		iClientRequest->Dispatch(tb, Gdb);


#ifdef GAME_TCP_SERVER_DEBUG
		iClientRequest->DebugShow(); /* ��ʾ���յ���ÿһ����Ϣ */
#endif
	}

	/* ��ɹ�����Ҫ�ǵ��ͷ��յ��� Request ���� */
	for (auto iClientRequest : iClientRequestList) {
		delete iClientRequest;
	}

	tb->GetTcpDataFromDumpedMessage(pTcpDataSend); /* ��ȡҪ�����͵���Ϣ */
	tb->ClearDumpedMessage();                      /* ��ջ����Ҫ���͵���Ϣ */
}
