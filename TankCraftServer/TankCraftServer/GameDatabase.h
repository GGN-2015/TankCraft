#pragma once
#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "GameGraph.h"
#include "MyMutex.h"

// TODO: LOCK BEFORE USE GAME_DATABASE

#define GAME_DATABASE_DEBUG
#define GAME_DATABASE_USER_MAX (8)

#define GAME_DATABASE_PHISICAL_FRAME_PERIOD \
  (5) /* 每 5ms 启动一次物理引擎   \
       */

#define GAME_DATABASE_RUN (1)
#define GAME_DATABASE_STOP (0)

#define TANK_SPEED (1.0)                          /* 每秒 1 格 */
#define TANK_ROTATE_SPEED (Utils::Get2PI() / 2.0) /* 每秒 0.5 圈 */
#define TANK_RADIUS (0.3)                         /* 坦克半径 */
#define WALL_WIDTH (0.05)                         /* 墙的宽度的一半 */
#define TANK_BULLET_RADIUS (0.04)                 /* 子弹的宽度 */
#define TANK_BULLET_EXPIRED_TIME (30.0)            /* 子弹生存时间 */

#define BULLET_SPEED (2 * TANK_SPEED) /* 子弹的速度 */

class BulletInfo;
class TcpData;
class UserInfo;

typedef std::vector<UserInfo*> UserInfoList;
typedef std::vector<BulletInfo> BulletInfoList;
typedef std::set<int> IntSet;
typedef std::map<int, int> IntMap;

/* GameDatabase 是整个游戏中最核心的单例 */
/* 使用这个数据库前需要手动加锁(我知道这很容易写错)，使用后需要手动解锁，不要和我叫板
 */
class GameDatabase : public MyMutex {
 public:
  ~GameDatabase();

  static GameDatabase* GetGlobalGameDatabase(); /* 获取单例 */
  bool CheckUserNameExist(
      std::wstring userName) const; /* 检测用户名是否已经被注册了 */

  int AllocNxtUserId();     /* 申请一个新的用户编号 */
  int GetUserCount() const; /* 获取服务器当前正在游玩的人数 */

  void AddUser(int nUserId, std::wstring nUserName); /* 将玩家加入游戏中 */
  void DelUser(int nUserId);                         /* 删除一个玩家 */

  void DealUserKilled(); /* 处理子弹杀死用户的情况 */

  /* alpha: 0 ~ 1 之间，描述地图的稠密程度: 0 稠密, 1 稀疏 */
  void GenerateNewMap(int mHeight, int mWidth,
                      double alpha); /* 生成一个新的地图 */
  void GetGraphTcpData(TcpData* mGraphTcpDataCache) const; /* 获取 地图数据 */

  double GetLastGraphGenerateTime() const; /* 获取上一次重置地图的时间 */
  void GetTcpDataForUserInfoMessage(TcpData* nTcpData); /* 获取用户数据 */

  void GetTankPosMessage(
      TcpData* pTcpData) const; /* 获取所有坦克当前的位置信息 */
  void GetBulletPosMessage(TcpData* pTcpData) const; /* 获取所有子弹的位置信息 */

  int GetMaxUserId() const;     /* 获得当前最大用户编号 */

  int GetGameDatabaseStatusAtomic() const;    /* 获取运行状态 */
  UserInfo* GetUserInfoByUserId(int nUserId); /* 获取用户信息对象 */

  /* TODO: 物理引擎线程 */
  static void GameDatabasePhsicalEngineThreadFunction(
      GameDatabase* pGameDatabase);

  static void
  GameDatabasePhsicalEngineTankFunction(/* 坦克物理引擎 */
                                        GameDatabase* pGameDatabase);

  static void
  GameDatabasePhsicalEngineBulletFunction(/* 子弹物理引擎 */
                                          GameDatabase* pGameDatabase);

  double GetLastFrameTime() const;
  void SetLastFrameTime(double nFrameTime);

  void SetKeyStatusForUser(int nUserId, int nKeyId,
                           bool status); /* 修改键盘状态 */

  /* 增加一个新的子弹 */
  void AddBullet(double posX, double posY, double dirR, double disD,
                 int userId);

  void GetCanShootUserIdSet(IntSet* userIdSet) const;
  void UserBulletExpired(IntMap* userIdMapToBulletCnt);

 private:
  GameDatabase();

  int mUserIdNow;                 /* 当前最大用户 ID */
  UserInfoList mUserInfoList;     /* 用户信息列表 */
  BulletInfoList mBulletInfoList; /* 子弹信息集合 */

  GameGraph mGameGraph;
  static GameDatabase* pGlobalGameDatabase; /* 指针单例 */

  double mLastFrameTime; /* 上次物理引擎启动时间 */

  int mGameDatabaseStatus;
  /* 在一个单独的线程运行物理引擎 */
  std::thread* pGameDatabasePhysicalEngineThread;
};
