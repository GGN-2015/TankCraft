#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <WinSock2.h> /* 使用 Socket.TCP 通信 */

#include "TcpServer.h"
#include "TcpData.h"
#include "Utils.h"

#define TCP_SERVER_DEBUG /* 注释这一行关闭调试模式 */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

/* static 函数实现一些需要使用 SOCKET 的功能封装 */
static void DealWithClient(SOCKET socketClient, TcpServer* tcpServer);
static void GetTcpDataFromClient(SOCKET socketClient, TcpData* tcpData);
static void SendTcpDataToClient(const TcpData* tcpData, SOCKET socketClient);
static void CreateProcessForClient(SOCKET socketClient, TcpServer* tcpServer);

TcpServer::~TcpServer()
{
}

/* 客户端线程主体函数 */
void DealWithClient(SOCKET socketClient, TcpServer* tcpServer)
{
	/* 如果客户端数量没有达到最大值 */
	/* 注统计的时候需要注意互斥问题 */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* 处理该客户端信息，统计数目 += 1 */
		tcpServer->IncClientCnt();
		while (true) {
			TcpData tcpDataRecv, tcpDataSend;

			/* 从客户端获取数据 */
			GetTcpDataFromClient(socketClient, &tcpDataRecv);
			if (tcpDataRecv.isEnd()) { /* 长度为零的消息是结束消息 */
				break;
			}

			/* 多态 */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
				SendTcpDataToClient(&tcpDataSend, socketClient);
			}
			else {
				break; /* 直接断开连接 */
			}
		}
		/* 统计数目 -= */
		tcpServer->DecClientCnt();
	}
	else {
		/* 用户数量过多，拒绝处理*/
	}

	/* 关闭连接 */
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
	/* 使用新的线程处理这个客户端的事件 */
	std::thread(DealWithClient, socketClient, tcpServer).detach();
}

TcpServer::TcpServer(const std::string& IP, int port): mIP(IP), mPort(port)
{
	mToken     = Utils::GetRandLongLong(); /* 生成随机 long long 数据作为服务器 Token */
	mRunStatus = TCP_SERVER_END;           /* 服务器不在运行 */
	mClientCnt = 0;                        /* 没有客户端在连接中 */
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
		CreateProcessForClient(socketClient, this);
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
