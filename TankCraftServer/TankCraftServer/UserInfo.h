#pragma once
#include <string>

/* ÿһ��������Ϸ��������û�����Ӧ��һ�� UserInfo, ����Ϣ�洢�� GameDatabase �� mUserInfoList ֮�� */
class UserInfo
{
public:
	UserInfo(int nUserId);
	int GetUserId() const; /* ��ȡ�û� ID */

	std::wstring GetUserName() const;
	void SetUserName(std::wstring nUserName);

private:
	int mUserId;
	std::wstring mUserName; /* ��¼�� */
};
