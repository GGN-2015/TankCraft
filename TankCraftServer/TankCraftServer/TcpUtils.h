#pragma once
#include "TankCraft_NetManager_Component.h"

#define TCP_BUFFER_MAX (1048576) /* TCP 的缓冲区大小 */

#define TCP_SERVER_CLIENT_MAX  (16) /* 最大连接数 */

#define TCP_SERVER_RUN (1) /* 服务器正在运行 */
#define TCP_SERVER_END (0) /* 服务器不在运行 */

#define TCP_SERVER_SUC        ( 0) /* 服务器成功停机 */
#define TCP_SERVER_FAIL       ( 1) /* 服务器启动失败 */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP 服务器的最大监听数 */

#define CLIENT_THREAD_SLEEP_TIME (60) /* 客户端发送消息的周期 ms for windows */

class TcpServer;
class TcpData;
class Xn::TankCraft::NetManager_Component;
class Xn::TankCraft::NetMessageBaseDataList;

class TcpUtils
{
public:
	/* static 函数实现一些需要使用 SOCKET 的功能封装 */
	static void DealWithClient(void* socketClient, TcpServer* tcpServer);
	static void GetTcpDataFromSocket(void* socketClient, TcpData* tcpData);
	static int  SendTcpDataToSocket(const TcpData* tcpData, void* socketClient); /* 可能返回 -1 */
	static void CreateProcessForClient(void* socketClient, TcpServer* tcpServer);

	/* 客户端接收数据的线程 */
	static void ClientThreadFunction(const char* ip, int port, Xn::TankCraft::NetManager_Component* nmComponent);
	
	/* 将多个 Request 消息打包成一个 Request 数据包 */
	static void CompactTcpDataListToTcpDataRequest(const TcpDataList* tcpDataList, TcpData* tcpDataRequest);

	/* 将一个数据包拆分成多个 Message*/
	static void UnpackTcpDataMessageToTcpDataList(const TcpData* pTcpDataMessage, TcpDataList* pTcpDataList);

	/* 将 tcpDataList 转换成 nmBaseDataList */
	static void GetTcpDataListFromNetMessageBaseDataList(Xn::TankCraft::NetMessageBaseDataList* nmBaseDataList, TcpDataList* tcpDataList);
};
