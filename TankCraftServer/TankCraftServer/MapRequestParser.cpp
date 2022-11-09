#include <cassert>

#include "MapRequest.h"
#include "MapRequestParser.h"
#include "Utils.h"

MapRequestParser::MapRequestParser():IParser(IPARSER_REGISTER_AT_CLINET_PARSER)
{
}

bool MapRequestParser::CheckAvailableToParse(const TcpData* pTcpData, int pos)
{
	return Utils::GetUnsignedShort(pTcpData->GetData(), pos) == 2; /* 地图消息 类型 = 2 */
}

IMessage* MapRequestParser::ParseAtRaw(const TcpData* pTcpData, int pos)
{
	assert(CheckAvailableToParse(pTcpData, pos));

	return new MapRequest;
}
