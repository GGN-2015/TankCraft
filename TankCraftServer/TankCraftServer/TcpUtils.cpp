#include <iostream>
#include <WinSock2.h>

#include "TcpData.h"
#include "TcpUtils.h"
#include "TcpServer.h"

/* �ͻ����߳����庯�� */
void TcpUtils::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	/* ����ͻ�������û�дﵽ���ֵ */
	/* עͳ�Ƶ�ʱ����Ҫע�⻥������ */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* ����ÿͻ�����Ϣ��ͳ����Ŀ += 1 */
		tcpServer->IncClientCnt();
		while (true) {
			TcpData tcpDataRecv, tcpDataSend;

			/* �ӿͻ��˻�ȡ���� */
			TcpUtils::GetTcpDataFromSocket(socketClient, &tcpDataRecv);
			if (tcpDataRecv.isEnd()) { /* ����Ϊ�����Ϣ�ǽ�����Ϣ */
				break;
			}

			/* ��̬ */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
				TcpUtils::SendTcpDataToSocket(&tcpDataSend, socketClient);
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

	int cnt = recv((SOCKET)socketClient, recvBuffer, sizeof(recvBuffer), 0);
	tcpData->SetData(recvBuffer, cnt);

	delete[] recvBuffer; // һ��Ҫ�ر�ע���ڴ�й©
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
	/* ʹ���µ��̴߳�������ͻ��˵��¼� */
	std::thread(DealWithClient, socketClient, tcpServer).detach();
}
