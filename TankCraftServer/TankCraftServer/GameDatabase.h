#pragma once
#include <vector>
#include <string>

#include "GameGraph.h"
#include "MyMutex.h"

// TODO: LOCK BEFORE USE GAME_DATABASE

#define GAME_DATABASE_DEBUG
#define GAME_DATABASE_USER_MAX (8)

class UserInfo;
typedef std::vector<UserInfo*> UserInfoList;

/* GameDatabase ��������Ϸ������ĵĵ��� */
/* ʹ��������ݿ�ǰ��Ҫ�ֶ�����(��֪���������д��)��ʹ�ú���Ҫ�ֶ���������Ҫ���ҽа� */
class GameDatabase: public MyMutex
{
public:
	static GameDatabase* GetGlobalGameDatabase(); /* ��ȡ���� */
	bool CheckUserNameExist(std::wstring userName) const; /* ����û����Ƿ��Ѿ���ע���� */

	int AllocNxtUserId(); /* ����һ���µ��û���� */
	int GetUserCount() const; /* ��ȡ��������ǰ������������� */

	void AddUser(int nUserId, std::wstring nUserName); /* ����Ҽ�����Ϸ�� */
	void DelUser(int nUserId); /* ɾ��һ����� */

	/* alpha: 0 ~ 1 ֮�䣬������ͼ�ĳ��̶ܳ�: 0 ����, 1 ϡ�� */
	void GenerateNewMap(int mHeight, int mWidth, double alpha); /* ����һ���µĵ�ͼ */
	void GetGraphTcpData(TcpData* mGraphTcpDataCache) const; /* ��ȡ ��ͼ���� */

	double GetLastGraphGenerateTime() const; /* ��ȡ��һ�����õ�ͼ��ʱ�� */
	void GetTcpDataForUserInfoMessage(TcpData* nTcpData); /* ��ȡ�û����� */

private:
	GameDatabase();

	int mUserIdNow; /* ��ǰ����û� ID */
	UserInfoList mUserInfoList; /* �û���Ϣ�б� */

	GameGraph mGameGraph;
	static GameDatabase* pGlobalGameDatabase; /* ָ�뵥�� */
};
