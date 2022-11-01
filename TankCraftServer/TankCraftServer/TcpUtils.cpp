#include <iostream>
#include <WinSock2.h>

#include "GameDatabase.h"
#include "TankCraft_NetManager_Component.h"
#include "TcpClient.h"
#include "TcpData.h"
#include "TcpUtils.h"
#include "TcpServer.h"
#include "ThreadBuffer.h"

/* 客户端线程主体函数 */
void TcpUtils::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	ThreadBuffer threadBuffer; /* 当前线程的缓存数据，用来储存对客户端的认识 */

	/* 如果客户端数量没有达到最大值 */
	/* 注统计的时候需要注意互斥问题 */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* 处理该客户端信息，统计数目 += 1 */
		tcpServer->IncClientCnt();
		while (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
			TcpData tcpDataRecv, tcpDataSend;

			/* 从客户端获取数据 */
			TcpUtils::GetTcpDataFromSocket(socketClient, &tcpDataRecv);
			if (tcpDataRecv.IsEnd()) { /* 长度为零的消息是结束消息 */
				break;
			}

			/* 多态 */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {

				/* 获取 TCP 的结果的同时可能会对服务器上的数据进行修改 */
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend, 
					&threadBuffer, GameDatabase::GetGlobalGameDatabase());
				
				int ret = TcpUtils::SendTcpDataToSocket(&tcpDataSend, socketClient);

				/* 数据发送失败，说明对方可能已经断开连接 */
				if (ret == -1) {
					break;
				}
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

	int cnt = recv((SOCKET)socketClient, recvBuffer, TCP_BUFFER_MAX, 0);
	tcpData->SetData(recvBuffer, cnt);

	delete[] recvBuffer; // 一定要特别注意内存泄漏
}

int TcpUtils::SendTcpDataToSocket(const TcpData* tcpData, void* socketClient)
{
	int ret = send((SOCKET)socketClient, tcpData->GetData(), tcpData->GetLength(), 0);
	return ret;
}

void TcpUtils::CreateProcessForClient(void* socketClient, TcpServer* tcpServer)
{
	/* 使用新的线程处理这个客户端的事件 */
	std::thread* pthread = new std::thread(DealWithClient, socketClient, tcpServer);
	pthread->detach();

	// TODO: 将 pthread 放入线程池并进行管理
}

void TcpUtils::ClientThreadFunction(const char* ip, int port, Xn::TankCraft::NetManager_Component* nmComponent)
{
	TcpClient tcpClient;

	int ret = tcpClient.Connect(ip, port);
	if (ret != 0) {
		nmComponent->PushFailedMessage(ret); /* 推送一个连接出错消息，告知出错原因 */
		return; /* 结束线程 */
	}
	else {
		nmComponent->PushSucessMessage(); /* 推送一个连接成功消息 */

		/* 线程主循环 */
		while (nmComponent->GetConnectStatus() == Xn::TankCraft::NetManager_Component::NET_MANAGER_ONLINE) {
			if (nmComponent->HasClientRequest()) {
				TcpDataList tcpDataList;

				/* 由于这个操作是原子的，所以要尽可能节省性能 */
				nmComponent->MoveClientRequestToTcpDataList(&tcpDataList);

				/* 对数据进行打包 */
				TcpData tcpDataRequest;
				TcpUtils::CompactTcpDataListToTcpDataRequest(&tcpDataList, &tcpDataRequest);

				/* 发送打包后的请求，获取消息 */
				TcpData tcpDataMessage;
				tcpClient.Request(&tcpDataRequest, &tcpDataMessage);

				/* 检测是否与服务器断开连接 */
				if (tcpDataMessage.IsEnd()) {
					nmComponent->PushFailedMessage(TCP_CLIENT_DISCONNECT_FROM_SERVER);
				}

				/* 对收到的消息进行解包 */
				tcpDataList.clear();
				TcpUtils::UnpackTcpDataMessageToTcpDataList(&tcpDataMessage, &tcpDataList);
			}

			Sleep(CLIENT_THREAD_SLEEP_TIME);
		}

		/* 断开连接 */
		tcpClient.CloseSocket();
	}
}
