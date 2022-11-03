#pragma once
#include "IParser.h"

class MapRequestParser :
    public IParser
{
public:
	MapRequestParser();
	virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) override;

private:
	virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) override;
};
