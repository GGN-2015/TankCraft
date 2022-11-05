#pragma once
#include <vector>

#include "IMessage.h"
#include "IParser.h"

/* 数据解析器 */
class DataParser {
 public:
  static const IParserList*
  GetGlobalClientRequestParserList(); /* 获取全局解析器列表 */
  static const IParserList* GetGlobalServerMessageParserList();

  static void RegisterToGlobalClientParser(
      IParser* iParser); /* 对解析器进行注册 */
  static void RegisterToGlobalServerParser(IParser* iParser);

  static void Parse(const TcpData* pTcpData, /* 解析一个 TcpData */
                    IMessageList* pContainer,
                    const IParserList* pGlobalParserList);

 private:
  static IParserList*
  GetMutableGlobalClientRequestParserList(); /* 获取全局解析器列表 */
  static IParserList* GetMutableGlobalServerMessageParserList();
};
