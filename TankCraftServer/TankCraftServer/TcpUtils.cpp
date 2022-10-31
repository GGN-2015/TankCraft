#include <iostream>
#include <WinSock2.h>

#include "GameDatabase.h"
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
	std::thread(DealWithClient, socketClient, tcpServer).detach();
}
