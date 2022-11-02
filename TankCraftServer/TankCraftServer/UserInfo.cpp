#include "UserInfo.h"

UserInfo::UserInfo(int nUserId)
{
    mUserId = nUserId;
}

int UserInfo::GetUserId() const
{
    return mUserId;
}

std::wstring UserInfo::GetUserName() const
{
    return mUserName;
}

void UserInfo::SetUserName(std::wstring nUserName)
{
    mUserName = nUserName;
}
