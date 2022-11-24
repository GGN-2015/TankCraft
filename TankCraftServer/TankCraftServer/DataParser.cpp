#include "DataParser.h"

#include <cassert>
#include <iostream>

#include "IMessage.h"
#include "IParser.h"
#include "TcpData.h"
#include "Utils.h"

const IParserList* DataParser::GetGlobalClientRequestParserList() {
  return GetMutableGlobalClientRequestParserList();
}

const IParserList* DataParser::GetGlobalServerMessageParserList() {
  return GetMutableGlobalServerMessageParserList();
}

IParserList* DataParser::GetMutableGlobalClientRequestParserList() {
  static IParserList globalClientRequestParserList; /* ��̬���� */
  return &globalClientRequestParserList;
}

IParserList* DataParser::GetMutableGlobalServerMessageParserList() {
  static IParserList globalServerMessageParserList; /* ��̬���� */
  return &globalServerMessageParserList;
}

void DataParser::RegisterToGlobalClientParser(IParser* iParser) {
  GetMutableGlobalClientRequestParserList()->push_back(iParser);
}

void DataParser::RegisterToGlobalServerParser(IParser* iParser) {
  GetMutableGlobalServerMessageParserList()->push_back(iParser);
}

void DataParser::Parse(const TcpData* pTcpData, IMessageList* pContainer,
                       const IParserList* pGlobalParserList) {
  pContainer->clear(); /* ����б� */
  assert(pTcpData->GetLength() >= 4 &&
         pTcpData->GetLength() % 2 == 0); /* ���ݳ��Ȳ����Ϲ淶 */
  assert(pTcpData->GetSanity());          /* ����ʹ��� */

  int dataLength = pTcpData->GetLength() - 2; /* ȥ��У����Ϣ��ĳ��� */
  int posNow = 2;
  int requestCnt = pTcpData->GetUnsignedShortAt(0); /* �ܵ��������� */
  int requestCntGet = 0;

  assert(pGlobalParserList->size() != 0); /* ����Ҫ��һ�������� */
  while (posNow < dataLength) {
    bool suc = false; /* �Ƿ�ɹ����� */
    for (IParser* iParser : (*pGlobalParserList)) {
      /* DEBUG OUTPUT */
      int type = Utils::GetUnsignedShort(pTcpData->GetData(), posNow);

      /* �����õ�����Ϣ���� */
      // if (type != 0) std::cerr << "Get Msg Type = " << type << std::endl;

      /* ���Կ�ʼ���� */
      if (iParser->CheckAvailableToParse(pTcpData, posNow)) {

        suc = true;
        requestCntGet += 1;

        IMessage* iMessage = iParser->ParseAt(
            pTcpData, posNow); /* ParseAt �������� IMessage ��� */
        posNow += iMessage->GetRawDataLength(); /* ����ʹ���߸����ͷ� */

        pContainer->push_back(
            std::shared_ptr<IMessage> (iMessage)); /* ����׷�ӵ�����β�� */
        break;
      }
    }

    assert(suc); /* ����ʧ����Ҫ���� */
  }

  char evenAns = 0, oddAns = 0;
  Utils::GetSanityInteger(pTcpData->GetData(), pTcpData->GetLength(), &evenAns,
                          &oddAns);

  assert(evenAns == 0 && oddAns == 0); /* У��� */
  assert(requestCntGet == requestCnt); /* ���ݰ���������ȷ */
  assert(posNow == dataLength); /* ���ݲ���û����ռУ����Ϣ */
}
