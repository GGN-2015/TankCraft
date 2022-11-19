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
  (5) /* ÿ 5ms ����һ����������   \
       */

#define GAME_DATABASE_RUN (1)
#define GAME_DATABASE_STOP (0)

#define TANK_SPEED (1.0)                          /* ÿ�� 1 �� */
#define TANK_ROTATE_SPEED (Utils::Get2PI() / 2.0) /* ÿ�� 0.5 Ȧ */
#define TANK_RADIUS (0.3)                         /* ̹�˰뾶 */
#define WALL_WIDTH (0.05)                         /* ǽ�Ŀ�ȵ�һ�� */
#define TANK_BULLET_RADIUS (0.04)                 /* �ӵ��Ŀ�� */
#define TANK_BULLET_EXPIRED_TIME (30.0)            /* �ӵ�����ʱ�� */

#define BULLET_SPEED (2 * TANK_SPEED) /* �ӵ����ٶ� */

class BulletInfo;
class TcpData;
class UserInfo;

typedef std::vector<UserInfo*> UserInfoList;
typedef std::vector<BulletInfo> BulletInfoList;
typedef std::set<int> IntSet;
typedef std::map<int, int> IntMap;

/* GameDatabase ��������Ϸ������ĵĵ��� */
/* ʹ��������ݿ�ǰ��Ҫ�ֶ�����(��֪���������д��)��ʹ�ú���Ҫ�ֶ���������Ҫ���ҽа�
 */
class GameDatabase : public MyMutex {
 public:
  ~GameDatabase();

  static GameDatabase* GetGlobalGameDatabase(); /* ��ȡ���� */
  bool CheckUserNameExist(
      std::wstring userName) const; /* ����û����Ƿ��Ѿ���ע���� */

  int AllocNxtUserId();     /* ����һ���µ��û���� */
  int GetUserCount() const; /* ��ȡ��������ǰ������������� */

  void AddUser(int nUserId, std::wstring nUserName); /* ����Ҽ�����Ϸ�� */
  void DelUser(int nUserId);                         /* ɾ��һ����� */

  void DealUserKilled(); /* �����ӵ�ɱ���û������ */

  /* alpha: 0 ~ 1 ֮�䣬������ͼ�ĳ��̶ܳ�: 0 ����, 1 ϡ�� */
  void GenerateNewMap(int mHeight, int mWidth,
                      double alpha); /* ����һ���µĵ�ͼ */
  void GetGraphTcpData(TcpData* mGraphTcpDataCache) const; /* ��ȡ ��ͼ���� */

  double GetLastGraphGenerateTime() const; /* ��ȡ��һ�����õ�ͼ��ʱ�� */
  void GetTcpDataForUserInfoMessage(TcpData* nTcpData); /* ��ȡ�û����� */

  void GetTankPosMessage(
      TcpData* pTcpData) const; /* ��ȡ����̹�˵�ǰ��λ����Ϣ */
  void GetBulletPosMessage(TcpData* pTcpData) const; /* ��ȡ�����ӵ���λ����Ϣ */

  int GetMaxUserId() const;     /* ��õ�ǰ����û���� */

  int GetGameDatabaseStatusAtomic() const;    /* ��ȡ����״̬ */
  UserInfo* GetUserInfoByUserId(int nUserId); /* ��ȡ�û���Ϣ���� */

  /* TODO: ���������߳� */
  static void GameDatabasePhsicalEngineThreadFunction(
      GameDatabase* pGameDatabase);

  static void
  GameDatabasePhsicalEngineTankFunction(/* ̹���������� */
                                        GameDatabase* pGameDatabase);

  static void
  GameDatabasePhsicalEngineBulletFunction(/* �ӵ��������� */
                                          GameDatabase* pGameDatabase);

  double GetLastFrameTime() const;
  void SetLastFrameTime(double nFrameTime);

  void SetKeyStatusForUser(int nUserId, int nKeyId,
                           bool status); /* �޸ļ���״̬ */

  /* ����һ���µ��ӵ� */
  void AddBullet(double posX, double posY, double dirR, double disD,
                 int userId);

  void GetCanShootUserIdSet(IntSet* userIdSet) const;
  void UserBulletExpired(IntMap* userIdMapToBulletCnt);

 private:
  GameDatabase();

  int mUserIdNow;                 /* ��ǰ����û� ID */
  UserInfoList mUserInfoList;     /* �û���Ϣ�б� */
  BulletInfoList mBulletInfoList; /* �ӵ���Ϣ���� */

  GameGraph mGameGraph;
  static GameDatabase* pGlobalGameDatabase; /* ָ�뵥�� */

  double mLastFrameTime; /* �ϴ�������������ʱ�� */

  int mGameDatabaseStatus;
  /* ��һ���������߳������������� */
  std::thread* pGameDatabasePhysicalEngineThread;
};
