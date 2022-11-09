#include <cassert>
#include <string>

#include "LoginRequest.h"
#include "LoginRequestParser.h"
#include "TcpData.h"
#include "Utils.h"

bool LoginRequestParser::CheckAvailableToParse(const TcpData* pTcpData, int pos)
{
    return pTcpData->GetUnsignedShortAt(pos) == REQUEST_TYPE_LOGIN_REQUEST;
}

IMessage* LoginRequestParser::ParseAtRaw(const TcpData* pTcpData, int pos)
{
    int userNameLen = pTcpData->GetUnsignedShortAt(pos + 2);
    int beginPos = pos + 4;

    assert(userNameLen > 0);
    assert(userNameLen % 2 == 0);

    char* buf = new char[userNameLen];
    for (int i = 0; i < userNameLen; i += 1) {
        buf[i] = pTcpData->GetData()[beginPos + i];
    }

    std::wstring userName = Utils::GetWstringFromCharBuffer(buf, userNameLen);

    LoginRequest* pLoginRequest = new LoginRequest;
    pLoginRequest->SetUserName(userName);
    delete[] buf;

    return pLoginRequest;
}
