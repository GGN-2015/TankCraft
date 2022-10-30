#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	/* ������������� */
	static long long GetRandLongLong();

	/* static ����ʵ��һЩ��Ҫʹ�� SOCKET �Ĺ��ܷ�װ */
	static void DealWithClient(void* socketClient, TcpServer* tcpServer);
	static void GetTcpDataFromSocket(void* socketClient, TcpData* tcpData);
	static void SendTcpDataToSocket(const TcpData* tcpData, void* socketClient);
	static void CreateProcessForClient(void* socketClient, TcpServer* tcpServer);
};
