#include "TcpData.h"
#include "UserInfo.h"
#include "Utils.h"

UserInfo::UserInfo(int nUserId)
{
    mUserId = nUserId;
    mKillCnt = 0;
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
