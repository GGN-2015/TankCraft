#pragma once
#include <string>

struct UserColor { /* 描述用户的颜色 */
	unsigned char R, G, B, A;
	UserColor() { R = G = B = 0; A = 255; } /* 默认颜色为黑色 */
	UserColor(unsigned char nR, unsigned char nG, unsigned char nB, unsigned char nA);

	static UserColor GetRandomColor();
};

class TcpData;

/* 每一个正在游戏中游玩的用户都对应着一个 UserInfo, 该信息存储与 GameDatabase 的 mUserInfoList 之中 */
class UserInfo
{
public:
	UserInfo(int nUserId);

	int GetUserId() const;                    /* 获取用户 ID */
	std::wstring GetUserName() const;         /* 获取用户名 */
	void SetUserName(std::wstring nUserName); /* 设置用户名 */
	void IncKillCnt(int incVal);              /* 新增击杀数 */

	/* 设置坦克颜色 */
	void SetUserColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A);
	void SetUserColor(UserColor uc);

	/* 获取用户对应的 TcpData 数据, 详见“用户信息格式” */
	void GetUserInfoTcpData(TcpData* tcpData) const;

private:
	int mUserId;
	std::wstring mUserName; /* 登录名 */

	int mKillCnt;
	UserColor mUserColor;
};
