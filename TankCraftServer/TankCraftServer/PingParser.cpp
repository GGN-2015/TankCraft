#include <cassert>
#include "PingParser.h"
#include "PingMessage.h"
#include "TcpData.h"

/* ��� Parser ����;��ע�� */
static PingParser* registerParser = new PingParser;

PingParser::PingParser(): IParser(IPARSER_REGISTER_AT_CLINET_PARSER)
{
}

bool PingParser::CheckAvailableToParse(const TcpData* pTcpData, int pos)
{
	return pTcpData->GetUnsignedShortAt(pos) == MESSAGE_TYPE_PING;
}

IMessage* PingParser::ParseAtRaw(const TcpData* pTcpData, int pos)
{
	assert(pos + 3 < pTcpData->GetLength());

	/* ��ȡ PingMessage */
	PingMessage* pPingMessage = new PingMessage();
	pPingMessage->SetX(pTcpData->GetUnsignedShortAt(pos + 2));

	return pPingMessage;
}
