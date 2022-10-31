#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <cassert>
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "TcpClient.h"
#include "TcpData.h"
#include "Utils.h"

TcpClient::TcpClient()
{
	pServerSocket = nullptr; /* δ���� */
}

TcpClient::~TcpClient()
{
	CloseSocket();
}

int TcpClient::Connect(const char* ip, int port)
{
	assert(pServerSocket == nullptr); /* û�гɹ����ӣ����� nullptr */

	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		std::cerr << "WSAStartup() error " << std::endl;
		return TCP_CLINET_STARTUP_ERR;
	}
	else {
		/* ���÷����*/
		SOCKADDR_IN addrServer;
		addrServer.sin_family = AF_INET;
		addrServer.sin_port = htons(port);
		addrServer.sin_addr.S_un.S_addr = inet_addr(ip);

		/* ���� socket */
		pServerSocket = (void*)socket(AF_INET, SOCK_STREAM, 0);
		if (pServerSocket == (void*)INVALID_SOCKET) {
			std::cerr << "socket() error " << std::endl;
			pServerSocket = nullptr;
			return TCP_CLINET_SOCKET_ERR;
		}

		/* �������� */
		if (connect((SOCKET)pServerSocket, (struct sockaddr*)&addrServer, sizeof(addrServer)) == INVALID_SOCKET)
		{
			std::cerr << "connect() error" << std::endl;
			return TCP_CLINET_CONNECT_ERR;
		}
	}

	return TCP_CLINET_CONNECT_SUC;
}

void TcpClient::Request(const TcpData* tcpDataSend, TcpData* tcpDataGet)
{
	assert(pServerSocket != nullptr);

	/* ������������ */
	send((SOCKET)pServerSocket, tcpDataSend->GetData(), tcpDataSend->GetLength(), 0);

	/* �� ServerSocket ����������Ϣ */
	char* recvBuff = new char[TCP_BUFFER_MAX];
	TcpUtils::GetTcpDataFromSocket(pServerSocket, tcpDataGet);

	delete[] recvBuff;
}

void TcpClient::CloseSocket() /* �ر� socket */
{
	if (pServerSocket != nullptr) {
		closesocket((SOCKET)pServerSocket);
		WSACleanup();
	}
	pServerSocket = nullptr;
}
