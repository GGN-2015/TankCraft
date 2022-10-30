#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <WinSock2.h> /* ʹ�� Socket.TCP ͨ�� */

#include "TcpServer.h"
#include "TcpData.h"
#include "Utils.h"

#define TCP_SERVER_DEBUG /* ע����һ�йرյ���ģʽ */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

/* static ����ʵ��һЩ��Ҫʹ�� SOCKET �Ĺ��ܷ�װ */
static void DealWithClient(SOCKET socketClient, TcpServer* tcpServer);
static void GetTcpDataFromClient(SOCKET socketClient, TcpData* tcpData);
static void SendTcpDataToClient(const TcpData* tcpData, SOCKET socketClient);
static void CreateProcessForClient(SOCKET socketClient, TcpServer* tcpServer);

TcpServer::~TcpServer()
{
}

/* �ͻ����߳����庯�� */
void DealWithClient(SOCKET socketClient, TcpServer* tcpServer)
{
	/* ����ͻ�������û�дﵽ���ֵ */
	/* עͳ�Ƶ�ʱ����Ҫע�⻥������ */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* ����ÿͻ�����Ϣ��ͳ����Ŀ += 1 */
		tcpServer->IncClientCnt();
		while (true) {
			TcpData tcpDataRecv, tcpDataSend;

			/* �ӿͻ��˻�ȡ���� */
			GetTcpDataFromClient(socketClient, &tcpDataRecv);
			if (tcpDataRecv.isEnd()) { /* ����Ϊ�����Ϣ�ǽ�����Ϣ */
				break;
			}

			/* ��̬ */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
				SendTcpDataToClient(&tcpDataSend, socketClient);
			}
			else {
				break; /* ֱ�ӶϿ����� */
			}
		}
		/* ͳ����Ŀ -= */
		tcpServer->DecClientCnt();
	}
	else {
		/* �û��������࣬�ܾ�����*/
	}

	/* �ر����� */
	closesocket(socketClient);
}

void GetTcpDataFromClient(SOCKET socketClient, TcpData* tcpData)
{
	char recvBuffer[TCP_SERVER_BUFFER_SIZE];
	int cnt = recv(socketClient, recvBuffer, sizeof(recvBuffer), 0);
	tcpData->SetData(recvBuffer, cnt);
}

static void SendTcpDataToClient(const TcpData* tcpData, SOCKET socketClient)
{
	int ret = send(socketClient, tcpData->GetData(), tcpData->GetLength(), 0);
	if (ret == SOCKET_ERROR) {
		std::cerr << "send() error" << std::endl;
		return;
	}
}

void CreateProcessForClient(SOCKET socketClient, TcpServer* tcpServer)
{
	/* ʹ���µ��̴߳�������ͻ��˵��¼� */
	std::thread(DealWithClient, socketClient, tcpServer).detach();
}

TcpServer::TcpServer(const std::string& IP, int port): mIP(IP), mPort(port)
{
	mToken     = Utils::GetRandLongLong(); /* ������� long long ������Ϊ������ Token */
	mRunStatus = TCP_SERVER_END;           /* �������������� */
	mClientCnt = 0;                        /* û�пͻ����������� */
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
		CreateProcessForClient(socketClient, this);
	}

	/* �رշ��������ͷ���Դ */
	closesocket(serverSocket);
	WSACleanup();
	return TCP_SERVER_SUC;
}

void TcpServer::GetTcpDataResult(const TcpData* request, TcpData* reply)
{
	/* �����õĻ�Ӧ������ֻ�ܷ��� HelloWorld */
	static const char msg[] = 
		"HTTP/1.0 200 OK\r\nContent-Type:text/html\r\nContent-Length: 14\r\n\r\nHello World!\r\n";

	reply->SetData(msg, strlen(msg));
}

long long TcpServer::GetToken() const
{
	return mToken;
}

int TcpServer::GetRunStatus() const
{
	return mRunStatus;
}

void TcpServer::SetRunStatus(int nStatus)
{
	mRunStatus = nStatus;
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
