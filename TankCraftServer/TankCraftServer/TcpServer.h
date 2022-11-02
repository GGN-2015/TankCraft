#pragma once
#include <string>
#include <thread>
#include <vector>

#include "MyMutex.h"
#include "TcpUtils.h"

class TcpData; /* TCP ͨ�ŵ����ݣ����ٰ���һ������һ������ */
class ThreadBuffer;
class GameDatabase;

class TcpServer
{
	friend TcpUtils;

public:
	TcpServer(const std::string& IP, int port); /* ����һ������������ */
	~TcpServer();

	int RunServer();                            /* �������������������ʧ�ܷ��� TCP_SERVER_SUC���ɹ��������� TCP_SERVER_FAIL */
	
	/* �û���Ҫ�Լ�ʵ��һ�� TcpData -> TcpData ֮���ӳ�� */
	virtual void GetTcpDataResult(const TcpData* request, TcpData* reply, ThreadBuffer* tb, GameDatabase* Gdb); 

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
