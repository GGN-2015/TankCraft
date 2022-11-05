#include <cassert>

#include "TcpData.h"
#include "UserInfo.h"
#include "Utils.h"

const double inf = 1e300;

UserInfo::UserInfo(int nUserId)
{
    mUserId = nUserId;
    mKillCnt = 0;
    mLastKilledTime = -inf;
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

void UserInfo::IncKillCnt(int incVal)
{
    mKillCnt += incVal;
}

void UserInfo::SetUserColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
    mUserColor.R = R;
    mUserColor.G = G;
    mUserColor.B = B;
    mUserColor.A = A;
}

void UserInfo::SetUserColor(UserColor uc)
{
    mUserColor = uc;
}

void UserInfo::GetUserInfoTcpData(TcpData* tcpData) const
{
    /* �û� ID, ��¼������, �ַ���, RGBA, ��ɱ�� */
    int dataLength = 4 + 2 + (2 * (int)mUserName.size()) + 4 + 2;
    int userNameLen = (2 * (int)mUserName.size());

    char* buf = new char[dataLength];
    Utils::DumpIntToBuffer(buf, 0, mUserId);                                     /* pos0: �û� ID 4 �ֽ� */
    Utils::DumpUnsignedShortToBuffer(buf, 4, (unsigned short)userNameLen);       /* pos4: ��¼������ 2 �ֽ� */
    Utils::DumpRawStringToBuffer(buf, 6, (char*)mUserName.c_str(), userNameLen); /* pos6: �ַ��� */
    
    int pos = 6 + userNameLen;
    buf[pos++] = *(char*)&mUserColor.R; /* ̹����ɫ */
    buf[pos++] = *(char*)&mUserColor.G;
    buf[pos++] = *(char*)&mUserColor.B;
    buf[pos++] = *(char*)&mUserColor.A;

    Utils::DumpUnsignedShortToBuffer(buf, pos, mKillCnt); /* ��ɱ�� */
    pos += 2;
    assert(pos == dataLength);

    tcpData->SetData(buf, dataLength);
    delete[] buf;
}

void UserInfo::GetTankPos(TankPosMap* nTankPosMap) /* ��̹��δ֪���� Map */
{
    (* nTankPosMap)[GetUserId()] = mTankPos;
}

void UserInfo::SetTankPos(const TankPosMap* nTankPosMap)
{
    auto pTankMsg = (*nTankPosMap).find(GetUserId());

    if (pTankMsg != nTankPosMap->end()) { /* �������û�ҵ���˵�����ܲ��� */
        mTankPos = pTankMsg->second;
    }
}

void UserInfo::SetTankPosRandomly(int mHeight, int mWidth)
{
    mTankPos.RandomPosition(mHeight, mWidth);
}

/* ̹��״̬: 0:��, 1:����, 2:�������� */
int UserInfo::GetTankStatus() const
{
    return 
        ((int)mHasLaserWeapon << 1) | (int)CheckSuperArmor();
}

bool UserInfo::CheckSuperArmor() const
{
    double timeNow = Utils::GetClockTime();
    if (timeNow < mLastKilledTime) {
        return false;
    }
    else {
        return timeNow - mLastKilledTime <= SUPER_ARMOR_TIME;
    }
}

void UserInfo::GetTankPosTcpData(TcpData* pTcpData) const
{
    char buf[18] = {};
    Utils::DumpIntToBuffer  ((char*)buf,  0, GetUserId());
    Utils::DumpFloatToBuffer((char*)buf,  4, mTankPos.posX);
    Utils::DumpFloatToBuffer((char*)buf,  8, mTankPos.posY);
    Utils::DumpFloatToBuffer((char*)buf, 12, mTankPos.dirR);
    Utils::DumpUnsignedShortToBuffer((char*)buf, 16, GetTankStatus());

    pTcpData->SetData(buf, 18);
}

UserColor::UserColor(unsigned char nR, unsigned char nG, unsigned char nB, unsigned char nA)
{
    R = nR;
    G = nG;
    B = nB;
    A = nA;
}

UserColor UserColor::GetRandomColor()
{
    unsigned char ucList[] = { 0, 0, 0 };
    for (int i = 0; i <= 2; i += 1) {
        ucList[i] = Utils::GetRandUnsignedChar();
    }
    return UserColor(ucList[0], ucList[1], ucList[2], 255);
}

TankPos::TankPos(const TankPos& nTankPos)
{
    posX = nTankPos.posX;
    posY = nTankPos.posY;
    dirR = nTankPos.dirR;
}

void TankPos::RandomPosition(int mHeight, int mWidth)
{
    posX = (Utils::GetRandLongLong() %  mWidth) + 0.5; /* ���λ�� */
    posY = (Utils::GetRandLongLong() % mHeight) + 0.5;
    dirR = Utils::GetRandomDouble() * 2 * acos(-1.0) ; /* ������� */
}
