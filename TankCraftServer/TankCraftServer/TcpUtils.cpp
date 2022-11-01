#include <iostream>
#include <WinSock2.h>

#include "GameDatabase.h"
#include "TankCraft_NetManager_Component.h"
#include "TcpClient.h"
#include "TcpData.h"
#include "TcpUtils.h"
#include "TcpServer.h"
#include "ThreadBuffer.h"

/* �ͻ����߳����庯�� */
void TcpUtils::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	ThreadBuffer threadBuffer; /* ��ǰ�̵߳Ļ������ݣ���������Կͻ��˵���ʶ */

	/* ����ͻ�������û�дﵽ���ֵ */
	/* עͳ�Ƶ�ʱ����Ҫע�⻥������ */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* ����ÿͻ�����Ϣ��ͳ����Ŀ += 1 */
		tcpServer->IncClientCnt();
		while (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
			TcpData tcpDataRecv, tcpDataSend;

			/* �ӿͻ��˻�ȡ���� */
			TcpUtils::GetTcpDataFromSocket(socketClient, &tcpDataRecv);
			if (tcpDataRecv.IsEnd()) { /* ����Ϊ�����Ϣ�ǽ�����Ϣ */
				break;
			}

			/* ��̬ */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {

				/* ��ȡ TCP �Ľ����ͬʱ���ܻ�Է������ϵ����ݽ����޸� */
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend, 
					&threadBuffer, GameDatabase::GetGlobalGameDatabase());
				
				int ret = TcpUtils::SendTcpDataToSocket(&tcpDataSend, socketClient);

				/* ���ݷ���ʧ�ܣ�˵���Է������Ѿ��Ͽ����� */
				if (ret == -1) {
					break;
				}
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
	closesocket((SOCKET)socketClient);
}

void TcpUtils::GetTcpDataFromSocket(void* socketClient, TcpData* tcpData)
{
	char* recvBuffer= new char[TCP_BUFFER_MAX];

	int cnt = recv((SOCKET)socketClient, recvBuffer, TCP_BUFFER_MAX, 0);
	tcpData->SetData(recvBuffer, cnt);

	delete[] recvBuffer; // һ��Ҫ�ر�ע���ڴ�й©
}

int TcpUtils::SendTcpDataToSocket(const TcpData* tcpData, void* socketClient)
{
	int ret = send((SOCKET)socketClient, tcpData->GetData(), tcpData->GetLength(), 0);
	return ret;
}

void TcpUtils::CreateProcessForClient(void* socketClient, TcpServer* tcpServer)
{
	/* ʹ���µ��̴߳�������ͻ��˵��¼� */
	std::thread* pthread = new std::thread(DealWithClient, socketClient, tcpServer);
	pthread->detach();

	// TODO: �� pthread �����̳߳ز����й���
}

void TcpUtils::ClientThreadFunction(const char* ip, int port, Xn::TankCraft::NetManager_Component* nmComponent)
{
	TcpClient tcpClient;

	int ret = tcpClient.Connect(ip, port);
	if (ret != 0) {
		nmComponent->PushFailedMessage(ret); /* ����һ�����ӳ�����Ϣ����֪����ԭ�� */
		return; /* �����߳� */
	}
	else {
		nmComponent->PushSucessMessage(); /* ����һ�����ӳɹ���Ϣ */

		/* �߳���ѭ�� */
		while (nmComponent->GetConnectStatus() == Xn::TankCraft::NetManager_Component::NET_MANAGER_ONLINE) {
			if (nmComponent->HasClientRequest()) {
				TcpDataList tcpDataList;

				/* �������������ԭ�ӵģ�����Ҫ�����ܽ�ʡ���� */
				nmComponent->MoveClientRequestToTcpDataList(&tcpDataList);

				/* �����ݽ��д�� */
				TcpData tcpDataRequest;
				TcpUtils::CompactTcpDataListToTcpDataRequest(&tcpDataList, &tcpDataRequest);

				/* ���ʹ��������󣬻�ȡ��Ϣ */
				TcpData tcpDataMessage;
				tcpClient.Request(&tcpDataRequest, &tcpDataMessage);

				/* ����Ƿ���������Ͽ����� */
				if (tcpDataMessage.IsEnd()) {
					nmComponent->PushFailedMessage(TCP_CLIENT_DISCONNECT_FROM_SERVER);
				}

				/* ���յ�����Ϣ���н�� */
				tcpDataList.clear();
				TcpUtils::UnpackTcpDataMessageToTcpDataList(&tcpDataMessage, &tcpDataList);
			}

			Sleep(CLIENT_THREAD_SLEEP_TIME);
		}

		/* �Ͽ����� */
		tcpClient.CloseSocket();
	}
}
