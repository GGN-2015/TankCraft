#pragma once
#include "TcpUtils.h"

class TcpData;
class TcpClient                                    
{
public:
	TcpClient(const char* ip, int port); /* ָ��Ҫ���ӵķ������� IP �Ͷ˿� */
	~TcpClient();
	void Request(const TcpData* tcpDataSend, TcpData* tcpDataGet);
	void CloseSocket();

private:
	void* pServerSocket;
};
