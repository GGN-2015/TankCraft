#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	/* 生成随机的整数 */
	static long long GetRandLongLong();

	/* static 函数实现一些需要使用 SOCKET 的功能封装 */
	static void DealWithClient(void* socketClient, TcpServer* tcpServer);
	static void GetTcpDataFromSocket(void* socketClient, TcpData* tcpData);
	static void SendTcpDataToSocket(const TcpData* tcpData, void* socketClient);
	static void CreateProcessForClient(void* socketClient, TcpServer* tcpServer);
};
