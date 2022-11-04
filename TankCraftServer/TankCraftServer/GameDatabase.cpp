#include <cassert>
#include <iostream>

#include "GameDatabase.h"
#include "GameGraph.h"
#include "UserInfo.h"
#include "Utils.h"

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
	pUserInfo->SetUserColor(UserColor::GetRandomColor());
	pUserInfo->SetTankPosRandomly(mGameGraph.GetHeight(), mGameGraph.GetWidth());

	mUserInfoList.push_back(pUserInfo); /* ����һ���û� */
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

void GameDatabase::GetGraphTcpData(TcpData* pGraphTcpDataCache) const
{
	mGameGraph.GetTcpData(pGraphTcpDataCache);
}

double GameDatabase::GetLastGraphGenerateTime() const
{
	return mGameGraph.GetCreateTime();
}

void GameDatabase::GetTcpDataForUserInfoMessage(TcpData* nTcpData)
{
	int userNameLenSum = 0;
	int userCnt = (int)mUserInfoList.size();

	for (auto pUserInfo : mUserInfoList) {
		userNameLenSum += 2 * (int)pUserInfo->GetUserName().size();
	}

	int subDataLen = userNameLenSum + userCnt * 12 + 2; /* �����ټ����û��������� 2 �ֽ� */
	int totalDataLen = 4 + subDataLen;

	char* buf = new char[totalDataLen];
	Utils::DumpUnsignedShortToBuffer(buf, 0, 3);          /* User Msg */
	Utils::DumpUnsignedShortToBuffer(buf, 2, subDataLen); /* �������ݳ��� */
	Utils::DumpUnsignedShortToBuffer(buf, 4, userCnt);    /* �û����� */

	int pos = 6;
	for (auto pUserInfo : mUserInfoList) {
		/* ��ȡһ���û������� */
		TcpData tmpTcpData;
		pUserInfo->GetUserInfoTcpData(&tmpTcpData);

		Utils::DumpTcpDataToBuffer(buf, pos, &tmpTcpData);
		pos += tmpTcpData.GetLength();
	}

	assert(pos == totalDataLen);
	nTcpData->SetData(buf, totalDataLen);
	delete[] buf;
}

void GameDatabase::GetTankPosMessage(TcpData* pTcpData) const
{
	int totalDataLen = 6 + 18 * (int)mUserInfoList.size();
	char* buf = new char[totalDataLen];
	Utils::DumpUnsignedShortToBuffer(buf, 0, 4); /* 4 ����Ϣ��ʾ̹��λ�� */
	Utils::DumpUnsignedShortToBuffer(buf, 2, totalDataLen - 4); /* ���ݲ��ֳ��� */
	Utils::DumpUnsignedShortToBuffer(buf, 4, (int)mUserInfoList.size()); /* ̹���� */
	int pos = 6;
	for (auto pUserInfo : mUserInfoList) {
		TcpData tmpTcpData;
		pUserInfo->GetTankPosTcpData(&tmpTcpData);
		Utils::DumpTcpDataToBuffer(buf, pos, &tmpTcpData);

		pos += tmpTcpData.GetLength();
		assert(tmpTcpData.GetLength() == 18);
	}
	assert(pos == totalDataLen);

	pTcpData->SetData(buf, totalDataLen); /* ��仰����д */
	delete[] buf;
}

int GameDatabase::GetMaxUserId() const
{
	return mUserIdNow;
}

GameDatabase::GameDatabase() {
	mUserIdNow = 0;
	mGameGraph.SetSize(16, 16, 0.4);
}
