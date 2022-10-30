#pragma once
#define TCP_CLIENT_BUFFER_MAX (4096) /* TCP �ͻ��˵���󻺳�����С */

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
          