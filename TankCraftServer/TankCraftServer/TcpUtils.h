#pragma once

#define TCP_BUFFER_MAX (1048576) /* TCP 的缓冲区大小 */

#define TCP_SERVER_CLIENT_MAX  (16) /* 最大连接数 */

#define TCP_SERVER_RUN (1) /* 服务器正在运行 */
#define TCP_SERVER_END (0) /* 服务器不在运行 */

#define TCP_SERVER_SUC        ( 0) /* 服务器成功停机 */
#define TCP_SERVER_FAIL       ( 1) /* 服务器启动失败 */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP 服务器的最大监听数 */

class TcpServer;
class TcpData;
class TcpUtils
{
public:
	/* static 函数实现一些需要使用 SOCKET 的功能封装 */
	static void DealWithClient(void* socketClient, TcpServer* tcpServer);
	static void GetTcpDataFromSocket(void* socketClient, TcpData* tcpData);
	static int  SendTcpDataToSocket(const TcpData* tcpData, void* socketClient); /* 可能返回 -1 */
	static void CreateProcessForClient(void* socketClient, TcpServer* tcpServer);
};
