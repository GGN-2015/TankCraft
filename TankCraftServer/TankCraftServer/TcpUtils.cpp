#include <iostream>
#include <WinSock2.h>

#include "TcpData.h"
#include "TcpUtils.h"
#include "TcpServer.h"

/* 客户端线程主体函数 */
void TcpUtils::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	/* 如果客户端数量没有达到最大值 */
	/* 注统计的时候需要注意互斥问题 */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* 处理该客户端信息，统计数目 += 1 */
		tcpServer->IncClientCnt();
		while (true) {
			TcpData tcpDataRecv, tcpDataSend;

			/* 从客户端获取数据 */
			TcpUtils::GetTcpDataFromSocket(socketClient, &tcpDataRecv);
			if (tcpDataRecv.isEnd()) { /* 长度为零的消息是结束消息 */
				break;
			}

			/* 多态 */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
				TcpUtils::SendTcpDataToSocket(&tcpDataSend, socketClient);
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
	closesocket((SOCKET)socketClient);
}

void TcpUtils::GetTcpDataFromSocket(void* socketClient, TcpData* tcpData)
{
	char* recvBuffer= new char[TCP_BUFFER_MAX];

	int cnt = recv((SOCKET)socketClient, recvBuffer, sizeof(recvBuffer), 0);
	tcpData->SetData(recvBuffer, cnt);

	delete[] recvBuffer; // 一定要特别注意内存泄漏
}

void TcpUtils::SendTcpDataToSocket(const TcpData* tcpData, void* socketClient)
{
	int ret = send((SOCKET)socketClient, tcpData->GetData(), tcpData->GetLength(), 0);
	if (ret == SOCKET_ERROR) {
		std::cerr << "send() error" << std::endl;
		return;
	}
}

void TcpUtils::CreateProcessForClient(void* socketClient, TcpServer* tcpServer)
{
	/* 使用新的线程处理这个客户端的事件 */
	std::thread(DealWithClient, socketClient, tcpServer).detach();
}
