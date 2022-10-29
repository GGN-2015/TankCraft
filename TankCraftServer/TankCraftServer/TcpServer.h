#pragma once
#include <string>

#define TCP_SERVER_RUN (1) /* 服务器正在运行 */
#define TCP_SERVER_END (0) /* 服务器不在运行 */

#define TCP_SERVER_SUC        ( 0) /* 服务器成功停机 */
#define TCP_SERVER_FAIL       ( 1) /* 服务器启动失败 */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP 服务器的最大监听数 */

class TcpData; /* TCP 通信的数据，至少包含一个串和一个长度 */

class TcpServer
{
public:
	/* 创建一个服务器对象 */
	TcpServer(const std::string& IP, int port);

	/* 启动服务器，如果启动失败返回 TCP_SERVER_SUC，成功结束返回 TCP_SERVER_FAIL */
	int RunServer();

	/* 用户需要自己实现一个 TcpData -> TcpData 之间的映射 */
	virtual void GetTcpDataResult(const TcpData* request, TcpData* reply);

	/* Token 是控制一个服务器的密码，启动时随机生成 */
	/* 管理员权限的操作(如：关机)需要校验 Token */
	long long GetToken() const;

	/* 获取当前 TCP 服务器的运行状态 */
	int GetRunStatus() const;

	/* 设置 TCP 服务器的运行状态 */
	void SetRunStatus(int nStatus);

private:
	/* 处理一个客户端 */
	void DealWithClient(SOCKET socketClient);

	/* 从客户端接收 TcpData */
	void GetTcpDataFromClient(SOCKET socketClient);

	/* 向客户端发送消息 */
	void SendTcpDataToClient(const TcpData* tcpData, SOCKET socketClient);

	/* 创建线程处理客户询问 */
	void CreateProcessForClient(SOCKET socketClient);

	long long   mToken;
	std::string mIP;
	int         mPort;
	int         mRunStatus;
};
