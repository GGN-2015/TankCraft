#pragma once
#include "MyMutex.h"

/* GameDatabase 是整个游戏中最核心的单例 */
class GameDatabase
{
public:
	static GameDatabase* GetGlobalGameDatabase(); /* 获取单例 */

private:
	GameDatabase();

	static GameDatabase* pGlobalGameDatabase; /* 指针单例 */
	MyMutex mMyMutex;                         /* 所有的写操作都需要加锁 */
};
