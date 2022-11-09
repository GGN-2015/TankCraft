#include "IMessage.h"
#include "KeyStatusRequest.h"
#include "KeyStatusRequestParser.h"
#include "Utils.h"

static KeyStatusRequestParser* registerKeyStatusRequestParser = new
KeyStatusRequestParser();

KeyStatusRequestParser::KeyStatusRequestParser()
    : IParser(IPARSER_REGISTER_AT_CLINET_PARSER) {}

bool KeyStatusRequestParser::CheckAvailableToParse(const TcpData* pTcpData,
                                                   int pos) {
  return Utils::GetUnsignedShort(pTcpData->GetData(), pos) == 6; /* °´¼üÏûÏ¢£º6 */
}

IMessage* KeyStatusRequestParser::ParseAtRaw(const TcpData* pTcpData, int pos) {
  KeyStatusRequest* pKeyStatusRequest = new KeyStatusRequest;
  pKeyStatusRequest->SetKeyId(pTcpData->GetData()[pos + 4]);
  pKeyStatusRequest->SetKeyStatus(pTcpData->GetData()[pos + 5]);
  return pKeyStatusRequest;
}
