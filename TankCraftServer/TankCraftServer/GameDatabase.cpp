#include <iostream>

#include "GameDatabase.h"
#include "UserInfo.h"

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
	return mUserInfoList.size();
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

GameDatabase::GameDatabase() {
	mUserIdNow = 0;
}
