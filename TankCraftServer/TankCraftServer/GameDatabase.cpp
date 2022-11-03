#include <iostream>

#include "GameDatabase.h"
#include "UserInfo.h"
#include <cassert>

GameDatabase* GameDatabase::pGlobalGameDatabase = nullptr;

GameDatabase* GameDatabase::GetGlobalGameDatabase()
{
	if (pGlobalGameDatabase == nullptr) {
		pGlobalGameDatabase = new GameDatabase;
	}
	return pGlobalGameDatabase;
}

bool GameDatabase::CheckUserNameExist(std::wstring userName) const
{
	bool ans = false;
	for (auto pUserInfo : mUserInfoList) {
		if (pUserInfo->GetUserName() == userName) {
			ans = true;
			break;
		}
	}
	return ans;
}

int GameDatabase::AllocNxtUserId()
{
	return ++ mUserIdNow;
}

int GameDatabase::GetUserCount() const
{
	return (int)mUserInfoList.size();
}

void GameDatabase::AddUser(int nUserId, std::wstring nUserName)
{
	UserInfo* pUserInfo = new UserInfo(nUserId);
	pUserInfo->SetUserName(nUserName);

	mUserInfoList.push_back(pUserInfo); /* 增加一个用户 */
#ifdef GAME_DATABASE_DEBUG
	std::wcerr << L"GameDatabase::AddUser() {\n    nUserId = " << nUserId <<
		L"\n    nUserName = " << nUserName << "\n}" << std::endl;
#endif
}

void GameDatabase::DelUser(int nUserId)
{
	int pos = -1;
	for (int i = 0; i < (int)mUserInfoList.size(); i += 1) {
		if (mUserInfoList[i]->GetUserId() == nUserId) {
			pos = i;
			break;
		}
	}

	assert(pos != -1);
	std::wcerr << L"[GameDatabase::DelUser] UserName = " << mUserInfoList[pos]->GetUserName() << std::endl;
	mUserInfoList.erase(mUserInfoList.begin() + pos);
}

void GameDatabase::GenerateNewMap(int mHeight, int mWidth, double alpha)
{
	mGameGraph.SetSize(mHeight, mWidth, alpha);
}

GameDatabase::GameDatabase() {
	mUserIdNow = 0;
}
