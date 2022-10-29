#pragma once
#include <string>

#define TCP_SERVER_RUN (1) /* �������������� */
#define TCP_SERVER_END (0) /* �������������� */

#define TCP_SERVER_SUC        ( 0) /* �������ɹ�ͣ�� */
#define TCP_SERVER_FAIL       ( 1) /* ����������ʧ�� */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP ���������������� */

class TcpData; /* TCP ͨ�ŵ����ݣ����ٰ���һ������һ������ */

class TcpServer
{
public:
	/* ����һ������������ */
	TcpServer(const std::string& IP, int port);

	/* �������������������ʧ�ܷ��� TCP_SERVER_SUC���ɹ��������� TCP_SERVER_FAIL */
	int RunServer();

	/* �û���Ҫ�Լ�ʵ��һ�� TcpData -> TcpData ֮���ӳ�� */
	virtual void GetTcpDataResult(const TcpData* request, TcpData* reply);

	/* Token �ǿ���һ�������������룬����ʱ������� */
	/* ����ԱȨ�޵Ĳ���(�磺�ػ�)��ҪУ�� Token */
	long long GetToken() const;

	/* ��ȡ��ǰ TCP ������������״̬ */
	int GetRunStatus() const;

	/* ���� TCP ������������״̬ */
	void SetRunStatus(int nStatus);

private:
	/* ����һ���ͻ��� */
	void DealWithClient(SOCKET socketClient);

	/* �ӿͻ��˽��� TcpData */
	void GetTcpDataFromClient(SOCKET socketClient);

	/* ��ͻ��˷�����Ϣ */
	void SendTcpDataToClient(const TcpData* tcpData, SOCKET socketClient);

	/* �����̴߳���ͻ�ѯ�� */
	void CreateProcessForClient(SOCKET socketClient);

	long long   mToken;
	std::string mIP;
	int         mPort;
	int         mRunStatus;
};
