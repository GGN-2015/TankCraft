#pragma once
#include <vector>

#include "IMessage.h"
#include "IParser.h"

/* ���ݽ����� */
class DataParser {
 public:
  static const IParserList*
  GetGlobalClientRequestParserList(); /* ��ȡȫ�ֽ������б� */
  static const IParserList* GetGlobalServerMessageParserList();

  static void RegisterToGlobalClientParser(
      IParser* iParser); /* �Խ���������ע�� */
  static void RegisterToGlobalServerParser(IParser* iParser);

  static void Parse(const TcpData* pTcpData, /* ����һ�� TcpData */
                    IMessageList* pContainer,
                    const IParserList* pGlobalParserList);

 private:
  static IParserList*
  GetMutableGlobalClientRequestParserList(); /* ��ȡȫ�ֽ������б� */
  static IParserList* GetMutableGlobalServerMessageParserList();
};
