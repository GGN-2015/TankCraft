#pragma once
#include <string>
#include <thread>
#include <vector>

#include "GameGraph.h"
#include "MyMutex.h"

// TODO: LOCK BEFORE USE GAME_DATABASE

#define GAME_DATABASE_DEBUG
#define GAME_DATABASE_USER_MAX (8)

#define GAME_DATABASE_PHISICAL_FRAME_PERIOD \
  (15) /* ÿ 15ms ����һ���������� \
        */

#define GAME_DATABASE_RUN (1)
#define GAME_DATABASE_STOP (0)

#define TANK_SPEED (1.0)  /* ÿ�� 1 �� */
#define TANK_ROTATE_SPEED (Utils::Get2PI()) /* ÿ��һȦ */
#define TANK_RADIUS (0.3) /* ̹�˰뾶 */

class UserInfo;
class TcpData;
typedef std::vector<UserInfo*> UserInfoList;

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

  /* alpha: 0 ~ 1 ֮�䣬������ͼ�ĳ��̶ܳ�: 0 ����, 1 ϡ�� */
  void GenerateNewMap(int mHeight, int mWidth,
                      double alpha); /* ����һ���µĵ�ͼ */
  void GetGraphTcpData(TcpData* mGraphTcpDataCache) const; /* ��ȡ ��ͼ���� */

  double GetLastGraphGenerateTime() const; /* ��ȡ��һ�����õ�ͼ��ʱ�� */
  void GetTcpDataForUserInfoMessage(TcpData* nTcpData); /* ��ȡ�û����� */

  void GetTankPosMessage(
      TcpData* pTcpData) const; /* ��ȡ����̹�˵�ǰ��λ����Ϣ */
  int GetMaxUserId() const;     /* ��õ�ǰ����û���� */

  int GetGameDatabaseStatusAtomic() const;    /* ��ȡ����״̬ */
  UserInfo* GetUserInfoByUserId(int nUserId); /* ��ȡ�û���Ϣ���� */

  /* TODO: ���������߳� */
  static void GameDatabasePhsicalEngineThreadFunction(
      GameDatabase* pGameDatabase);

  double GetLastFrameTime() const;
  void SetLastFrameTime(double nFrameTime);

  void SetKeyStatusForUser(int nUserId, int nKeyId,
                           bool status); /* �޸ļ���״̬ */

 private:
  GameDatabase();

  int mUserIdNow;             /* ��ǰ����û� ID */
  UserInfoList mUserInfoList; /* �û���Ϣ�б� */

  GameGraph mGameGraph;
  static GameDatabase* pGlobalGameDatabase; /* ָ�뵥�� */

  double mLastFrameTime; /* �ϴ�������������ʱ�� */

  int mGameDatabaseStatus;
  /* ��һ���������߳������������� */
  std::thread* pGameDatabasePhysicalEngineThread;
};
