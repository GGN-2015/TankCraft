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
	static IParserList globalClientRequestParserList; /* 静态单例 */
	return &globalClientRequestParserList;
}

IParserList* DataParser::GetMutableGlobalServerMessageParserList()
{
	static IParserList globalServerMessageParserList; /* 静态单例 */
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
	pContainer->clear(); /* 清空列表 */
	assert(pTcpData->GetLength() >= 4 && pTcpData->GetLength() % 2 == 0); /* 数据长度不符合规范 */
	assert(pTcpData->GetSanity()); /* 检验和错误 */

	int dataLength = pTcpData->GetLength() - 2; /* 去除校验信息后的长度 */
	int posNow = 2;
	int requestCnt = (int)Utils::GetUnsignedShort(pTcpData->GetData(), 0); /* 总的请求数量 */
	int requestCntGet = 0;
	while (posNow < dataLength) {
		bool suc = false; /* 是否成功解析 */
		for (IParser* iParser : (*pGlobalParserList)) {

			/* 可以开始解析 */
			if (iParser->CheckAvailableToParse(pTcpData, posNow)) {
				suc = true;
				requestCntGet += 1;

				IMessage* iMessage = iParser->ParseAt(pTcpData, posNow); /* ParseAt 负责申请 IMessage 结点 */
				posNow += iMessage->GetRawDataLength();                  /* 数据使用者负责释放 */

				pContainer->push_back(iMessage);                         /* 数据追加到队列尾部 */
			}
		}

		assert(suc); /* 解析失败需要报错 */
	}

	/* 数据包个数不正确 */
	assert(requestCntGet == requestCnt);
}
