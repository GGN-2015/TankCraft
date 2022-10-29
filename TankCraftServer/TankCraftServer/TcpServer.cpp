#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <WinSock2.h> /* ʹ�� Socket.TCP ͨ�� */

#include "TcpServer.h"
#include "Utils.h"

TcpServer::TcpServer(const std::string& IP, int port): mIP(IP), mPort(port)
{
	/* ������� long long ������Ϊ������ Token */
	mToken     = Utils::GetRandLongLong();
	mRunStatus = TCP_SERVER_END; /* �������������� */
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
		CreateProcessForClient(socketClient);
	}

	/* �رշ��������ͷ���Դ */
	closesocket(serverSocket);
	WSACleanup();
	return TCP_SERVER_SUC;
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

void TcpServer::DealWithClient(SOCKET socketClient)
{
	while (true) {
		TcpData tcpDataRecv, tcpDataSend;

		/* �ӿͻ��˻�ȡ���� */
		GetTcpDataFromClient(socketClient, &tcpDataRecv);
		if (tcpDataRecv.isEnd()) { /* ����Ϊ�����Ϣ�ǽ�����Ϣ */
			break;
		}

		/* ��̬ */
		GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
		SendTcpDataToClient(&tcpDataSend, socketClient);
	}

	/* �ر����� */
	closesocket(socketClient);
}
