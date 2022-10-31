#pragma once
#include "IParser.h"
#include "RequestMessageTypes.h"

/* 客户发送的 Ping 请求，给定整数 X，要求服务器计算 X + 1 */
class PingParser :
    public IParser
{
public:
	PingParser();
	virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) override;

private:
	virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) override;
};
