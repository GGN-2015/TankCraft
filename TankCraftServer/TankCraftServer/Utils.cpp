#include <cstdlib>
#include <iostream>
#include <string.h>
#include <WinSock2.h>

#include "Utils.h"

long long Utils::GetRandLongLong()
{
	long long ans = 0;
	for (int i = 1; i <= 4; i++) {
		ans = (ans << 15) | rand(); /* �˴���ֻ�� windows ƽ̨���� */
	}
	return ans;
}


/* �ͻ����߳����庯�� */
void Utils::DealWithClient(void* socketClient, TcpServer* tcpServer)
{
	/* ����ͻ�������û�дﵽ���ֵ */
	/* עͳ�Ƶ�ʱ����Ҫע�⻥������ */
	if (tcpServer->GetClientCnt() < TCP_SERVER_CLIENT_MAX) {

		/* ����ÿͻ�����Ϣ��ͳ����Ŀ += 1 */
		tcpServer->IncClientCnt();
		while (true) {
			TcpData tcpDataRecv, tcpDataSend;

			/* �ӿͻ��˻�ȡ���� */
			Utils::GetTcpDataFromSocket(socketClient, &tcpDataRecv);
			if (tcpDataRecv.isEnd()) { /* ����Ϊ�����Ϣ�ǽ�����Ϣ */
				break;
			}

			/* ��̬ */
			if (tcpServer->GetRunStatus() == TCP_SERVER_RUN) {
				tcpServer->GetTcpDataResult(&tcpDataRecv, &tcpDataSend);
				Utils::SendTcpDataToSocket(&tcpDataSend, socketClient);
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

void Utils::GetTcpDataFromSocket(void* socketClient, TcpData* tcpData)
{
	char recvBuffer[TCP_SERVER_BUFFER_SIZE];
	int cnt = recv((SOCKET)socketClient, recvBuffer, sizeof(recvBuffer), 0);
	tcpData->SetData(recvBuffer, cnt);
}

void Utils::SendTcpDataToSocket(const TcpData* tcpData, void* socketClient)
{
	int ret = send((SOCKET)socketClient, tcpData->GetData(), tcpData->GetLength(), 0);
	if (ret == SOCKET_ERROR) {
		std::cerr << "send() error" << std::endl;
		return;
	}
}

void Utils::CreateProcessForClient(void* socketClient, TcpServer* tcpServer)
{
	/* ʹ���µ��̴߳�������ͻ��˵��¼� */
	std::thread(DealWithClient, socketClient, tcpServer).detach();
}
