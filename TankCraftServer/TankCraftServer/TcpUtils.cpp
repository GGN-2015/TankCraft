#include <iostream>
#include <WinSock2.h>

#include "GameDatabase.h"
#include "TankCraft_NetManager_Component.h"
#include "TcpClient.h"
#include "TcpData.h"
#include "TcpUtils.h"
#include "TcpServer.h"
#include "ThreadBuffer.h"
#include "Utils.h"

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

void TcpUtils::GetTcpDataFromSocket(void* socketClient, TcpData* tcpData)
{
	char* recvBuffer= new char[TCP_BUFFER_MAX];

	int cnt = recv((SOCKET)socketClient, recvBuffer, TCP_BUFFER_MAX, 0);
	tcpData->SetData(recvBuffer, cnt);

	delete[] recvBuffer; // 一定要特别注意内存泄漏
}

int TcpUtils::SendTcpDataToSocket(const TcpData* tcpData, void* socketClient)
{
	tcpData->DebugShow("[SendToClient] ");
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

void TcpUtils::ClientThreadFunction(std::string ip, int port, Xn::TankCraft::NetManager_Component* nmComponent)
{
	TcpClient tcpClient;

	int ret = tcpClient.Connect(ip.c_str(), port);
	
	std::cerr << "[Client] Start with " << ip << ":" << port << std::endl;

	if (ret != 0) {
		nmComponent->PushFailedMessage(ret); /* 推送一个连接出错消息，告知出错原因 */

		std::cerr << "[Client] End with ErrorLevel =  " << ret << std::endl;
		return; /* 结束线程 */
	}
	else {
		nmComponent->PushSucessMessage(); /* 推送一个连接成功消息 */

		/* 线程主循环 */
		while (nmComponent->GetConnectStatus() == Xn::TankCraft::NetManager_Component::NET_MANAGER_ONLINE) {
			
			/* 如果有请求则直接请求 */
			if (nmComponent->HasClientRequest()) {
				std::cerr << "[Client] has request to send." << std::endl;

				TcpDataList tcpDataList;
				Xn::TankCraft::NetMessageBaseDataList nmBaseDataList;

				/* 由于这个操作是原子的，所以要尽可能节省性能 */
				nmComponent->MoveClientRequestToNetMessageBaseDataList(&nmBaseDataList);
				TcpUtils::GetTcpDataListFromNetMessageBaseDataList(&nmBaseDataList, &tcpDataList);

				/* 对数据进行打包 */
				TcpData tcpDataRequest;
				TcpUtils::CompactTcpDataListToTcpDataRequest(&tcpDataList, &tcpDataRequest);


				/* 发送打包后的请求，获取消息 */
				int sendTime = clock();
				TcpData tcpDataMessage;
				tcpClient.Request(&tcpDataRequest, &tcpDataMessage);
				int getTime = clock();

				// TODO: 存储 Ping 值


				/* 检测是否与服务器断开连接 */
				if (tcpDataMessage.IsEnd()) {
					nmComponent->PushFailedMessage(TCP_CLIENT_DISCONNECT_FROM_SERVER);
					std::cerr << "[Client] Can not connect to server." << std::endl;
				}

				/* 释放先前的 Request 空间 */
				for (auto pTcpData : tcpDataList) { 
					delete pTcpData;
				}
				tcpDataList.clear();

				/* 对收到的消息进行解包 */
				if (!tcpDataMessage.IsEnd()) {
					TcpUtils::UnpackTcpDataMessageToTcpDataList(&tcpDataMessage, &tcpDataList);

					/* 把拆分后的数据送入消息队列 */
					for (auto pTcpData : tcpDataList) {
						nmComponent->PushServerMessageTcpData(pTcpData);
					}

					for (auto pTcpData : tcpDataList) { /* 释放 Message 空间 */
						delete pTcpData;
					}
					tcpDataList.clear();
				}
				else break;
			}
			else {
				/* 如果没有请求也要发送一个 ping 保证自己能够刷新地图 */
				unsigned short xVal = Utils::GetRandLongLong() % 65536;
				nmComponent->PushPingMessage(xVal);
			}

			Sleep(CLIENT_THREAD_SLEEP_TIME);
		}

		/* 断开连接 */
		tcpClient.CloseSocket();
		std::cerr << "[Client End]" << std::endl;
	}
}


void TcpUtils::CompactTcpDataListToTcpDataRequest(const TcpDataList* tcpDataList, TcpData* tcpDataRequest) {
	int totalLength = 4; /* 头部信息两字节，校验信息两字节 */
	int totalCnt = (int)tcpDataList->size();

	/* 计算二进制信息总长度 */
	for (auto pTcpData : *tcpDataList) {
		totalLength += pTcpData->GetLength();
	}

	/* 申请缓冲区 */
	char* buf = new char[totalLength];
	int pos = 0;
	Utils::DumpUnsignedShortToBuffer(buf, pos, (unsigned short)totalCnt);
	pos += 2;

	/* 将数据填写进入消息 */
	for (auto pTcpData : *tcpDataList) {
		Utils::DumpTcpDataToBuffer(buf, pos, pTcpData);
		pos += pTcpData->GetLength();
	}
	assert(pos == totalLength - 2); /* 检查是否恰好还剩两个位置 */

	/* 计算校验和 */
	char evenAns = 0, oddAns = 0;
	Utils::GetSanityInteger(buf, pos, &evenAns, &oddAns);
	buf[pos + 0] = evenAns;
	buf[pos + 1] = oddAns;

	tcpDataRequest->SetData(buf, totalLength);
	delete[] buf;
}

void TcpUtils::UnpackTcpDataMessageToTcpDataList(const TcpData* pTcpDataMessage, TcpDataList* pTcpDataList) {
	assert(pTcpDataMessage->GetSanity());
	int messageTotalLength = pTcpDataMessage->GetLength() - 2;

	/* 取出消息总数 */
	int pos = 0;
	unsigned short messageCnt = Utils::GetUnsignedShort(pTcpDataMessage->GetData(), pos);
	int cnt = 0;
	pos += 2;

	while (pos < messageTotalLength) {
		/* 从 pos 开始长度为 dataLength 的一段是一个消息 */
		unsigned short dataLength = Utils::GetUnsignedShort(pTcpDataMessage->GetData(), pos + 2) + 4;

		pTcpDataMessage->DebugShow("[TcpData] ");
		assert(pos + dataLength <= messageTotalLength);

		TcpData* tcpDataNow = new TcpData;
		tcpDataNow->SetData(pTcpDataMessage->GetData() + pos, dataLength);
		pTcpDataList->push_back(tcpDataNow); /* 主函数负责释放 */

		cnt += 1;
		pos += dataLength;
	}

	assert(pos == messageTotalLength);
	assert(cnt == messageCnt);
}

void TcpUtils::GetTcpDataListFromNetMessageBaseDataList(
	Xn::TankCraft::NetMessageBaseDataList* nmBaseDataList, TcpDataList* tcpDataList) {
	tcpDataList->clear();

	for (int i = 0; i < (int)nmBaseDataList->size(); i += 1) {

		TcpData* tcpDataNow = new TcpData;
		(*nmBaseDataList)[i]->MoveDataToTcpData(tcpDataNow); /* 移动数据 */

		/* TODO 要记得释放 */
		tcpDataList->push_back(tcpDataNow);
	}
}
