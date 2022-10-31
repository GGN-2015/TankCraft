#include "UserInfo.h"

UserInfo::UserInfo(int nUserId)
{
    mUserId = nUserId;
}

std::wstring UserInfo::GetUserName() const
{
    return mUserName;
}

void UserInfo::SetUserName(std::wstring nUserName)
{
    mUserName = nUserName;
}
