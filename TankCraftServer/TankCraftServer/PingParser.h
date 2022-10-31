#pragma once
#include "IParser.h"

#define MESSAGE_TYPE_PING (0)

/* �ͻ����͵� Ping ���󣬸������� X��Ҫ����������� X + 1 */
class PingParser :
    public IParser
{
public:
	PingParser();
	virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) override;

private:
	virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) override;
};
