#pragma once
#include "IParser.h"
#include "RequestMessageTypes.h"

class LoginRequestParser :
    public IParser
{
public:
    LoginRequestParser() : IParser(IPARSER_REGISTER_AT_CLINET_PARSER) {}

    /* �˺���ֻ��Ҫ���ͷ��ǩ�Ƿ���ȷ */
    virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos);

private:
    virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos);
};
