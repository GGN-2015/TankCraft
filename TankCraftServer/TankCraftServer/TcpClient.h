#pragma once
#include "TcpUtils.h"

class TcpData;
class TcpClient                                    
{
public:
	TcpClient(const char* ip, int port); /* 指明要连接的服务器的 IP 和端口 */
	~TcpClient();
	void Request(const TcpData* tcpDataSend, TcpData* tcpDataGet);
	void CloseSocket();

private:
	void* pServerSocket;
};
