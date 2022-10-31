#include <iostream>

#include "DataParser.h"
#include "IMessage.h"
#include "IParser.h"
#include "TcpData.h"
#include "Utils.h"
#include <cassert>

const IParserList* DataParser::GetGlobalClientRequestParserList()
{
	return GetMutableGlobalClientRequestParserList();
}

const IParserList* DataParser::GetGlobalServerMessageParserList()
{
	return GetMutableGlobalServerMessageParserList();
}

IParserList* DataParser::GetMutableGlobalClientRequestParserList()
{
	static IParserList globalClientRequestParserList; /* ��̬���� */
	return &globalClientRequestParserList;
}

IParserList* DataParser::GetMutableGlobalServerMessageParserList()
{
	static IParserList globalServerMessageParserList; /* ��̬���� */
	return &globalServerMessageParserList;
}

void DataParser::RegisterToGlobalClientParser(IParser* iParser)
{
	GetMutableGlobalClientRequestParserList()->push_back(iParser);
}

void DataParser::RegisterToGlobalServerParser(IParser* iParser)
{
	GetMutableGlobalServerMessageParserList()->push_back(iParser);
}

void DataParser::Parse(const TcpData* pTcpData, IMessageList* pContainer, const IParserList* pGlobalParserList)
{
	pContainer->clear(); /* ����б� */
	assert(pTcpData->GetLength() >= 4 && pTcpData->GetLength() % 2 == 0); /* ���ݳ��Ȳ����Ϲ淶 */
	assert(pTcpData->GetSanity()); /* ����ʹ��� */

	int dataLength = pTcpData->GetLength() - 2; /* ȥ��У����Ϣ��ĳ��� */
	int posNow = 2;
	int requestCnt = (int)Utils::GetUnsignedShort(pTcpData->GetData(), 0); /* �ܵ��������� */
	int requestCntGet = 0;
	while (posNow < dataLength) {
		bool suc = false; /* �Ƿ�ɹ����� */
		for (IParser* iParser : (*pGlobalParserList)) {

			/* ���Կ�ʼ���� */
			if (iParser->CheckAvailableToParse(pTcpData, posNow)) {
				suc = true;
				requestCntGet += 1;

				IMessage* iMessage = iParser->ParseAt(pTcpData, posNow); /* ParseAt �������� IMessage ��� */
				posNow += iMessage->GetRawDataLength();                  /* ����ʹ���߸����ͷ� */

				pContainer->push_back(iMessage);                         /* ����׷�ӵ�����β�� */
			}
		}

		assert(suc); /* ����ʧ����Ҫ���� */
	}

	/* ���ݰ���������ȷ */
	assert(requestCntGet == requestCnt);
}
