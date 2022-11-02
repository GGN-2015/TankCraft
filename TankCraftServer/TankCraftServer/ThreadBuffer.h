#pragma once
#include "IMessage.h"

/* 本类的实现不需要考虑线程安全，因为我们保证每个线程各自有一个 ThreadBuffer */
class ThreadBuffer
{
public:
	ThreadBuffer();

	void DumpMessage(IMessage* iMessage); /* 向缓冲区中缓冲一条消息 */
	void ClearDumpedMessage();            /* 清空所有待发送的消息 */

	void GetTcpDataFromDumpedMessage(TcpData* pTcpDataGet); /* 获取要被发送的 TcpData */
	void SetUserID(int nID);                                /* 登录检测通过时获得用户 ID */

	bool InGame() const; /* 检测当前该客户是否在游玩 */
	int GetUserID() const; /* 获取用户编号 */

private:
	IMessageList mIMessageList; /* 待发送的消息队列，服务端有义务在清空这个队列时对其中内容进行释放 */

	/* 描述当前玩家是否已经加入游戏 */
	/* 未加入的玩家可以通过登录请求加入游戏，登录请求失败的仍然不能加入游戏 */
	/* 刚建立连接的用户处于观战模式，ID = -1，不能操纵坦克 */
	/* 已经登录的用户再次登录，该消息会被忽略 */
	int mUserID;

	double mLastKillListTime; /* 上一次获取 KillList 的时间 */
	double mLastShootTime;    /* 上次发射炮弹的时间 */
};
