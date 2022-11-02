#pragma once
#include <string>

/* 每一个正在游戏中游玩的用户都对应着一个 UserInfo, 该信息存储与 GameDatabase 的 mUserInfoList 之中 */
class UserInfo
{
public:
	UserInfo(int nUserId);
	int GetUserId() const; /* 获取用户 ID */

	std::wstring GetUserName() const;
	void SetUserName(std::wstring nUserName);

private:
	int mUserId;
	std::wstring mUserName; /* 登录名 */
};
