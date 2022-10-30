#pragma once
#include <string>
#include <thread>
#include <vector>

#include "MyMutex.h"

#define TCP_SERVER_BUFFER_SIZE (4096) /* 一次通信的最大数据量 */
#define TCP_SERVER_CLIENT_MAX  (16) /* 最大连接数 */

#define TCP_SERVER_RUN (1) /* 服务器正在运行 */
#define TCP_SERVER_END (0) /* 服务器不在运行 */

#define TCP_SERVER_SUC        ( 0) /* 服务器成功停机 */
#define TCP_SERVER_FAIL       ( 1) /* 服务器启动失败 */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP 服务器的最大监听数 */

class TcpData; /* TCP 通信的数据，至少包含一个串和一个长度 */

class TcpServer
{
public:
	TcpServer(const std::string& IP, int port); /* 创建一个服务器对象 */
	~TcpServer();

	int RunServer();                            /* 启动服务器，如果启动失败返回 TCP_SERVER_SUC，成功结束返回 TCP_SERVER_FAIL */
	virtual void GetTcpDataResult(const TcpData* request, TcpData* reply); /* 用户需要自己实现一个 TcpData -> TcpData 之间的映射 */
	long long GetToken() const;                 /* Token 是控制一个服务器的密码，启动时随机生成, 管理员权限的操作(如：关机)需要校验 Token */	
	int GetRunStatus() const;                   /* 获取当前 TCP 服务器的运行状态 */
	void SetRunStatus(int nStatus);             /* 设置 TCP 服务器的运行状态 */

	/* 这三个函数不是给各位用的，虽然是 public 的，但是没事别乱调用 */
	int GetClientCnt() const;                   /* 获取当前用户数量 */
	void IncClientCnt();                        /* 增加一个客户 */
	void DecClientCnt();                        /* 减少一个客户 */

private:
	long long    mToken;
	std::string  mIP;
	int          mPort;
	volatile int mRunStatus;
	int          mClientCnt; 
	MyMutex      mMyMutex;
};
