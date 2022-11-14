#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")

#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h> /* 使用 Socket.TCP 通信 */

#include "GameDatabase.h"
#include "TcpServer.h"
#include "TcpData.h"
#include "ThreadBuffer.h"
#include "Utils.h"

#define TCP_SERVER_DEBUG                /* 注释这一行关闭调试模式 */


TcpServer::~TcpServer()
{
}

/* 客户端线程主体函数 */
void TcpServer::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	ThreadBuffer threadBuffer; /* 当前线程的缓存数据，用来储存对客户端的认识 */

	/* 如果客户端数量没有达到最大值 */
	/* 注统计的时候需要注意互斥问题 */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* 处理该客户端信息，统计数目 += 1 */
		tcpServer->IncClientCnt();
		while (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
			// TcpData tcpDataRecv, tcpDataSend;
            std::shared_ptr<TcpData> tcpDataRecv(
                TcpData::AllocTcpData(__FILE__, __LINE__, false));

            std::shared_ptr<TcpData> tcpDataSend(
                TcpData::AllocTcpData(__FILE__, __LINE__, false));

			/* 从客户端获取数据 */
			TcpUtils::GetTcpDataFromSocket(socketClient, tcpDataRecv.get());
			if (tcpDataRecv->IsEnd()) { /* 长度为零的消息是结束消息 */
				break;
			}

			/* 多态 */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {

				/* 获取 TCP 的结果的同时可能会对服务器上的数据进行修改 */
				tcpServer->GetTcpDataResult(tcpDataRecv.get(), tcpDataSend.get(),
					&threadBuffer, GameDatabase::GetGlobalGameDatabase());

				int ret = TcpUtils::SendTcpDataToSocket(tcpDataSend.get(), socketClient);

				/* 数据发送失败，说明对方可能已经断开连接 */
				if (ret == -1) {
					break;
				}
			}
			else {
				break; /* 直接断开连接 */
			}
		}

		/* 让游戏中的玩家下线 */
		if (threadBuffer.InGame()) {
			/* 一定要记得用前上锁 */
			GameDatabase::GetGlobalGameDatabase()->lock();
			GameDatabase::GetGlobalGameDatabase()->DelUser(threadBuffer.GetUserID());
			GameDatabase::GetGlobalGameDatabase()->unlock();
		}

		/* 统计数目 -= */
		tcpServer->DecClientCnt();
	}
	else {
		/* 用户数量过多，拒绝处理*/
	}

	/* 关闭连接 */
	closesocket((SOCKET)socketClient);
}


void TcpServer::CreateProcessForClient(void* socketClient, TcpServer* tcpServer)
{
	/* 使用新的线程处理这个客户端的事件 */
	std::thread* pthread = new std::thread(DealWithClient, socketClient, tcpServer);
	pthread->detach();

	// TODO: 将 pthread 放入线程池并进行管理
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
		TcpServer::CreateProcessForClient((void*)socketClient, this);
	}

	/* 关闭服务器并释放资源 */
	closesocket(serverSocket);
	WSACleanup();
	return TCP_SERVER_SUC;
}

void TcpServer::GetTcpDataResult(const TcpData* request, TcpData* reply, ThreadBuffer* tb, GameDatabase* Gdb)
{
	/* 测试功能，将字符串翻转再发回去 */
	int len = request->GetLength();

	/* 拷贝数据 */
	char* buffer = new char[len];
	memcpy(buffer, request->GetData(), request->GetLength());
	std::reverse(buffer, buffer + len);

	/* 发送数据 */
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
