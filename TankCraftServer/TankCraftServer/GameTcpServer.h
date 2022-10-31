#pragma once
#include "TcpServer.h"

class TcpData;
class GameTcpServer: public TcpServer
{
public:
	GameTcpServer(const char* ip, int port); /* ������Ϸ������ */

	virtual void GetTcpDataResult(const TcpData* pTcpDataRecv, 
		TcpData* pTcpDataSend, ThreadBuffer* tb, GameDatabase* Gdb) override;
};
