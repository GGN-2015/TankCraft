#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <WinSock2.h> /* 使用 Socket.TCP 通信 */

#include "TcpServer.h"
#include "Utils.h"

TcpServer::TcpServer(const std::string& IP, int port): mIP(IP), mPort(port)
{
	/* 生成随机 long long 数据作为服务器 Token */
	mToken     = Utils::GetRandLongLong();
	mRunStatus = TCP_SERVER_END; /* 服务器不在运行 */
}

int TcpServer::RunServer() {
	SOCKET  serverSocket = INVALID_SOCKET; /* 创建 socket 对象 */
	WSADATA serverWsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &serverWsaData);
	if (result != 0) {
		/* 服务端启动失败 */
		std::cerr << "WSAStartup() init error " << GetLastError() << std::endl;
		return TCP_SERVER_FAIL;
	}

	/* 配置 ip 和端口结构体 */
	serverSocket = socket(AF_INET, SOCK_STREAM, 0); /* Socket.TCP */
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(mPort);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(mIP.c_str());

	/* 绑定 ip 和端口 */
	result = bind(serverSocket, (LPSOCKADDR)&serverAddr, sizeof(SOCKADDR_IN));
	if (result != 0) {
		std::cerr << "bind() error " << result << std::endl;
		return TCP_SERVER_FAIL;
	}

	/* 设置最大监听数 */
	result = listen(serverSocket, TCP_SERVER_MAX_LISTEN);
	if (result != 0) {
		std::cerr << "listen() error " << result << std::endl;
		return TCP_SERVER_FAIL;
	}

	/* 开始监听客户连接 */
	SetRunStatus(TCP_SERVER_RUN);
	while (GetRunStatus() == TCP_SERVER_RUN) {
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(SOCKADDR);

		/* accept 等待一个客户连接 */
		SOCKET socketClient = accept(serverSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (socketClient == SOCKET_ERROR) {

			/* 错误处理 */
			std::cerr << "accept() error " << WSAGetLastError() << std::endl;
			SetRunStatus(TCP_SERVER_END);
			return TCP_SERVER_FAIL;
		}

		/* 创建线程处理客户询问 */
		CreateProcessForClient(socketClient);
	}

	/* 关闭服务器并释放资源 */
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

		/* 从客户端获取数据 */
		GetTcpDataFromClient(socketClient, &tcpDataRecv);
		if (tcpDataRecv.isEnd()) { /* 长度为零的消息是结束消息 */
			break;
		}

		/* 多态 */
		GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
		SendTcpDataToClient(&tcpDataSend, socketClient);
	}

	/* 关闭连接 */
	closesocket(socketClient);
}
