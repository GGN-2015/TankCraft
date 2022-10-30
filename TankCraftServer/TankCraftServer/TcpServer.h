#pragma once
#include <string>
#include <thread>
#include <vector>

#include "MyMutex.h"

#define TCP_SERVER_BUFFER_SIZE (4096) /* һ��ͨ�ŵ���������� */
#define TCP_SERVER_CLIENT_MAX  (16) /* ��������� */

#define TCP_SERVER_RUN (1) /* �������������� */
#define TCP_SERVER_END (0) /* �������������� */

#define TCP_SERVER_SUC        ( 0) /* �������ɹ�ͣ�� */
#define TCP_SERVER_FAIL       ( 1) /* ����������ʧ�� */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP ���������������� */

class TcpData; /* TCP ͨ�ŵ����ݣ����ٰ���һ������һ������ */

class TcpServer
{
public:
	TcpServer(const std::string& IP, int port); /* ����һ������������ */
	~TcpServer();

	int RunServer();                            /* �������������������ʧ�ܷ��� TCP_SERVER_SUC���ɹ��������� TCP_SERVER_FAIL */
	virtual void GetTcpDataResult(const TcpData* request, TcpData* reply); /* �û���Ҫ�Լ�ʵ��һ�� TcpData -> TcpData ֮���ӳ�� */
	long long GetToken() const;                 /* Token �ǿ���һ�������������룬����ʱ�������, ����ԱȨ�޵Ĳ���(�磺�ػ�)��ҪУ�� Token */	
	int GetRunStatus() const;                   /* ��ȡ��ǰ TCP ������������״̬ */
	void SetRunStatus(int nStatus);             /* ���� TCP ������������״̬ */

	/* �������������Ǹ���λ�õģ���Ȼ�� public �ģ�����û�±��ҵ��� */
	int GetClientCnt() const;                   /* ��ȡ��ǰ�û����� */
	void IncClientCnt();                        /* ����һ���ͻ� */
	void DecClientCnt();                        /* ����һ���ͻ� */

private:
	long long    mToken;
	std::string  mIP;
	int          mPort;
	volatile int mRunStatus;
	int          mClientCnt; 
	MyMutex      mMyMutex;
};
