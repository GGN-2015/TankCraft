#pragma once
#include "TcpUtils.h"

#define TCP_CLINET_CONNECT_SUC (0)
#define TCP_CLINET_STARTUP_ERR (1)
#define TCP_CLINET_SOCKET_ERR  (2)
#define TCP_CLINET_CONNECT_ERR (3)

class TcpData;
class TcpClient                                    
{
public:
	TcpClient(); /* 指明要连接的服务器的 IP 和端口 */
	~TcpClient();

	int  Connect(const char* ip, int port);
	void Request(const TcpData* tcpDataSend, TcpData* tcpDataGet);
	void CloseSocket();

private:
	void* pServerSocket;
};
