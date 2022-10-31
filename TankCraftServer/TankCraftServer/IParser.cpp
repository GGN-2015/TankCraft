#include <cassert>

#include "DataParser.h"
#include "IParser.h"
#include "TcpData.h"

IParser::IParser(int registerPos)
{
    /* 选择注册位置 */
    assert(
        registerPos == IPARSER_REGISTER_AT_CLINET_PARSER ||
        registerPos == IPARSER_REGISTER_AT_SERVER_PARSER
    );

    switch (registerPos) {

    case IPARSER_REGISTER_AT_CLINET_PARSER: /* 注册到客户端请求解析器集合 */
        DataParser::RegisterToGlobalClientParser(this);
        break;

    case IPARSER_REGISTER_AT_SERVER_PARSER: /* 注册到服务端消息解析器集合 */
        DataParser::RegisterToGlobalServerParser(this);
        break;
    }
}

IMessage* IParser::ParseAt(const TcpData* pTcpData, int pos)
{
    assert(CheckAvailableToParse(pTcpData, pos));
    assert(pos < pTcpData->GetLength());
    return ParseAtRaw(pTcpData, pos);
}
