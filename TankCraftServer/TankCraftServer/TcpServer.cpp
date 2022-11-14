#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> /* ʹ�� Socket.TCP ͨ�� */

#include "GameDatabase.h"
#include "TcpServer.h"
#include "TcpData.h"
#include "ThreadBuffer.h"
#include "Utils.h"

#define TCP_SERVER_DEBUG                /* ע����һ�йرյ���ģʽ */


TcpServer::~TcpServer()
{
}

/* �ͻ����߳����庯�� */
void TcpServer::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	ThreadBuffer threadBuffer; /* ��ǰ�̵߳Ļ������ݣ���������Կͻ��˵���ʶ */

	/* ����ͻ�������û�дﵽ���ֵ */
	/* עͳ�Ƶ�ʱ����Ҫע�⻥������ */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* ����ÿͻ�����Ϣ��ͳ����Ŀ += 1 */
		tcpServer->IncClientCnt();
		while (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
			// TcpData tcpDataRecv, tcpDataSend;
            std::shared_ptr<TcpData> tcpDataRecv(
                TcpData::AllocTcpData(__FILE__, __LINE__, false));

            std::shared_ptr<TcpData> tcpDataSend(
                TcpData::AllocTcpData(__FILE__, __LINE__, false));

			/* �ӿͻ��˻�ȡ���� */
			TcpUtils::GetTcpDataFromSocket(socketClient, tcpDataRecv.get());
			if (tcpDataRecv->IsEnd()) { /* ����Ϊ�����Ϣ�ǽ�����Ϣ */
				break;
			}

			/* ��̬ */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {

				/* ��ȡ TCP �Ľ����ͬʱ���ܻ�Է������ϵ����ݽ����޸� */
				tcpServer->GetTcpDataResult(tcpDataRecv.get(), tcpDataSend.get(),
					&threadBuffer, GameDatabase::GetGlobalGameDatabase());

				int ret = TcpUtils::SendTcpDataToSocket(tcpDataSend.get(), socketClient);

				/* ���ݷ���ʧ�ܣ�˵���Է������Ѿ��Ͽ����� */
				if (ret == -1) {
					break;
				}
			}
			else {
				break; /* ֱ�ӶϿ����� */
			}
		}

		/* ����Ϸ�е�������� */
		if (threadBuffer.InGame()) {
			/* һ��Ҫ�ǵ���ǰ���� */
			GameDatabase::GetGlobalGameDatabase()->lock();
			GameDatabase::GetGlobalGameDatabase()->DelUser(threadBuffer.GetUserID());
			GameDatabase::GetGlobalGameDatabase()->unlock();
		}

		/* ͳ����Ŀ -= */
		tcpServer->DecClientCnt();
	}
	else {
		/* �û��������࣬�ܾ�����*/
	}

	/* �ر����� */
	closesocket((SOCKET)socketClient);
}


void TcpServer::CreateProcessForClient(void* socketClient, TcpServer* tcpServer)
{
	/* ʹ���µ��̴߳�������ͻ��˵��¼� */
	std::thread* pthread = new std::thread(DealWithClient, socketClient, tcpServer);
	pthread->detach();

	// TODO: �� pthread �����̳߳ز����й���
}


TcpServer::TcpServer(const std::string& IP, int port): mIP(IP), mPort(port)
{
	mToken     = Utils::GetRandLongLong(); /* ������� long long ������Ϊ������ Token */
	mRunStatus = TCP_SERVER_END;           /* �������������� */
	mClientCnt = 0;                        /* û�пͻ����������� */

	/* ��ʾ Token */
	std::cerr << "[TcpServer] TcpServer() Token = " << mToken << std::endl;
}

int TcpServer::RunServer() {
	SOCKET  serverSocket = INVALID_SOCKET; /* ���� socket ���� */
	WSADATA serverWsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &serverWsaData);
	if (result != 0) {
		/* ���������ʧ�� */
		std::cerr << "WSAStartup() init error " << GetLastError() << std::endl;
		return TCP_SERVER_FAIL;
	}

	/* ���� ip �Ͷ˿ڽṹ�� */
	serverSocket = socket(AF_INET, SOCK_STREAM, 0); /* Socket.TCP */
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(mPort);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(mIP.c_str());

	/* �� ip �Ͷ˿� */
	result = bind(serverSocket, (LPSOCKADDR)&serverAddr, sizeof(SOCKADDR_IN));
	if (result != 0) {
		std::cerr << "bind() error " << result << std::endl;
		return TCP_SERVER_FAIL;
	}

	/* ������������ */
	result = listen(serverSocket, TCP_SERVER_MAX_LISTEN);
	if (result != 0) {
		std::cerr << "listen() error " << result << std::endl;
		return TCP_SERVER_FAIL;
	}

	/* ��ʼ�����ͻ����� */
	SetRunStatus(TCP_SERVER_RUN);
	while (GetRunStatus() == TCP_SERVER_RUN) {
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(SOCKADDR);

		/* accept �ȴ�һ���ͻ����� */
		SOCKET socketClient = accept(serverSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (socketClient == SOCKET_ERROR) {

			/* ������ */
			std::cerr << "accept() error " << WSAGetLastError() << std::endl;
			SetRunStatus(TCP_SERVER_END);
			return TCP_SERVER_FAIL;
		}

		/* �����̴߳���ͻ�ѯ�� */
		TcpServer::CreateProcessForClient((void*)socketClient, this);
	}

	/* �رշ��������ͷ���Դ */
	closesocket(serverSocket);
	WSACleanup();
	return TCP_SERVER_SUC;
}

void TcpServer::GetTcpDataResult(const TcpData* request, TcpData* reply, ThreadBuffer* tb, GameDatabase* Gdb)
{
	/* ���Թ��ܣ����ַ�����ת�ٷ���ȥ */
	int len = request->GetLength();

	/* �������� */
	char* buffer = new char[len];
	memcpy(buffer, request->GetData(), request->GetLength());
	std::reverse(buffer, buffer + len);

	/* �������� */
	reply->SetData(buffer, len);
	delete[] buffer;
}

long long TcpServer::GetToken() const
{
	return mToken;
}

int TcpServer::GetRunStatus() const
{
	mMyMutex.lock();
	int ans = mRunStatus;
	mMyMutex.unlock();
	return ans;
}

void TcpServer::SetRunStatus(int nStatus)
{
	mMyMutex.lock();
	mRunStatus = nStatus;
	mMyMutex.unlock();
}

int TcpServer::GetClientCnt() const
{
	mMyMutex.lock(); /* �����ٽ��� */
	int ans = mClientCnt;
	mMyMutex.unlock(); /* �˳��ٽ��� */
	return ans;
}

void TcpServer::IncClientCnt()
{
	mMyMutex.lock(); /* �����ٽ��� */
	mClientCnt += 1;
#ifdef TCP_SERVER_DEBUG
	std::cerr << "IncClientCnt() mClientCnt = " << mClientCnt << std::endl;
#endif
	mMyMutex.unlock(); /* �˳��ٽ��� */
}

void TcpServer::DecClientCnt()
{
	mMyMutex.lock(); /* �����ٽ��� */
	mClientCnt -= 1;
#ifdef TCP_SERVER_DEBUG
	std::cerr << "DecClientCnt() mClientCnt = " << mClientCnt << std::endl;
#endif
	mMyMutex.unlock(); /* �˳��ٽ��� */
}
