#pragma once
#define TCP_CLIENT_BUFFER_MAX (4096) /* TCP 客户端的最大缓冲区大小 */

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
          