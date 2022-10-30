#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> /* 使用 Socket.TCP 通信 */

#include "TcpServer.h"
#include "TcpData.h"
#include "Utils.h"

#define TCP_SERVER_DEBUG                /* 注释这一行关闭调试模式 */


TcpServer::~TcpServer()
{
}

TcpServer::TcpServer(const std::string& IP, int port): mIP(IP), mPort(port)
{
	mToken     = Utils::GetRandLongLong(); /* 生成随机 long long 数据作为服务器 Token */
	mRunStatus = TCP_SERVER_END;           /* 服务器不在运行 */
	mClientCnt = 0;                        /* 没有客户端在连接中 */

	/* 显示 Token */
	std::cerr << "[TcpServer] TcpServer() Token = " << mToken << std::endl;
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
		Utils::CreateProcessForClient((void*)socketClient, this);
	}

	/* 关闭服务器并释放资源 */
	closesocket(serverSocket);
	WSACleanup();
	return TCP_SERVER_SUC;
}

void TcpServer::GetTcpDataResult(const TcpData* request, TcpData* reply)
{
	/* 测试用的回应函数，只能发送 HelloWorld */
	static const char msg[] = 
		"HTTP/1.0 200 OK\r\nContent-Type:text/html\r\nContent-Length: 23\r\n\r\n<h1>Hello World!</h1>\r\n";

	reply->SetData(msg, strlen(msg));
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
	mMyMutex.lock(); /* 进入临界区 */
	int ans = mClientCnt;
	mMyMutex.unlock(); /* 退出临界区 */
	return ans;
}

void TcpServer::IncClientCnt()
{
	mMyMutex.lock(); /* 进入临界区 */
	mClientCnt += 1;
#ifdef TCP_SERVER_DEBUG
	std::cerr << "IncClientCnt() mClientCnt = " << mClientCnt << std::endl;
#endif
	mMyMutex.unlock(); /* 退出临界区 */
}

void TcpServer::DecClientCnt()
{
	mMyMutex.lock(); /* 进入临界区 */
	mClientCnt -= 1;
#ifdef TCP_SERVER_DEBUG
	std::cerr << "DecClientCnt() mClientCnt = " << mClientCnt << std::endl;
#endif
	mMyMutex.unlock(); /* 退出临界区 */
}
