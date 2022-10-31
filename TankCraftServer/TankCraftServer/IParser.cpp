#include <cassert>

#include "DataParser.h"
#include "IParser.h"
#include "TcpData.h"

IParser::IParser(int registerPos)
{
    /* ѡ��ע��λ�� */
    assert(
        registerPos == IPARSER_REGISTER_AT_CLINET_PARSER ||
        registerPos == IPARSER_REGISTER_AT_SERVER_PARSER
    );

    switch (registerPos) {

    case IPARSER_REGISTER_AT_CLINET_PARSER: /* ע�ᵽ�ͻ���������������� */
        DataParser::RegisterToGlobalClientParser(this);
        break;

    case IPARSER_REGISTER_AT_SERVER_PARSER: /* ע�ᵽ�������Ϣ���������� */
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
