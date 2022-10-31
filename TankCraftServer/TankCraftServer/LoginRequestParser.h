#pragma once
#include "IParser.h"
#include "RequestMessageTypes.h"

class LoginRequestParser :
    public IParser
{
public:
    LoginRequestParser() : IParser(IPARSER_REGISTER_AT_CLINET_PARSER) {}

    /* 此函数只需要检查头标签是否正确 */
    virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos);

private:
    virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos);
};
