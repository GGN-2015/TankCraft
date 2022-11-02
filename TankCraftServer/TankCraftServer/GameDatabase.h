#pragma once
#include <vector>
#include <string>
#include "MyMutex.h"

// TODO: LOCK BEFORE USE GAME_DATABASE

#define GAME_DATABASE_DEBUG
#define GAME_DATABASE_USER_MAX (8)

class UserInfo;
typedef std::vector<UserInfo*> UserInfoList;

/* GameDatabase 是整个游戏中最核心的单例 */
/* 使用这个数据库前需要手动加锁(我知道这很容易写错)，使用后需要手动解锁，不要和我叫板 */
class GameDatabase: public MyMutex
{
public:
	static GameDatabase* GetGlobalGameDatabase(); /* 获取单例 */
	bool CheckUserNameExist(std::wstring userName) const; /* 检测用户名是否已经被注册了 */

	int AllocNxtUserId(); /* 申请一个新的用户编号 */
	int GetUserCount() const; /* 获取服务器当前正在游玩的人数 */

	void AddUser(int nUserId, std::wstring nUserName); /* 将玩家加入游戏中 */
	void DelUser(int nUserId); /* 删除一个玩家 */

private:
	GameDatabase();

	int mUserIdNow; /* 当前最大用户 ID */
	UserInfoList mUserInfoList; /* 用户信息列表 */

	static GameDatabase* pGlobalGameDatabase; /* 指针单例 */
};
