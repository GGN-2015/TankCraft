#include "UserInfoRequest.h"
#include "UserInfoRequestParser.h"
#include "Utils.h"

UserInfoRequestParser::UserInfoRequestParser(): IParser(IPARSER_REGISTER_AT_CLINET_PARSER)
{
}

bool UserInfoRequestParser::CheckAvailableToParse(const TcpData* pTcpData, int pos)
{
	return Utils::GetUnsignedShort(pTcpData->GetData(), pos) == 3; /* 用户信息 */
}

IMessage* UserInfoRequestParser::ParseAtRaw(const TcpData* pTcpData, int pos)
{
	return new UserInfoRequest;
}
