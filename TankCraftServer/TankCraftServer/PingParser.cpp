#include <cassert>
#include "PingParser.h"
#include "PingMessage.h"
#include "TcpData.h"

/* 这个 Parser 的用途是注册 */
static PingParser* registerParser = new PingParser;

PingParser::PingParser(): IParser(IPARSER_REGISTER_AT_CLINET_PARSER)
{
}

bool PingParser::CheckAvailableToParse(const TcpData* pTcpData, int pos)
{
	return pTcpData->GetUnsignedShortAt(pos) == REQUEST_TYPE_PING;
}

IMessage* PingParser::ParseAtRaw(const TcpData* pTcpData, int pos)
{
	assert(pos + 3 < pTcpData->GetLength());

	/* 获取 PingMessage */
	int dataLen = pTcpData->GetUnsignedShortAt(pos + 2);
	assert(dataLen == 2);

	PingMessage* pPingMessage = new PingMessage();
	pPingMessage->SetX(pTcpData->GetUnsignedShortAt(pos + 4));

	return pPingMessage;
}
