#pragma once
#include <vector>

class TcpData;
class IMessage;
class IParser;

#define IPARSER_REGISTER_AT_CLINET_PARSER \
  (0) /* 将自身注册到客户请求解析器集合 */
#define IPARSER_REGISTER_AT_SERVER_PARSER \
  (1) /* 将自身注册到服务消息解析器集合*/

typedef std::vector<IParser*> IParserList; /* 解析器列表 */

class IParser {
 public:
  /* 每个 IParser 会在实例化的时候自动注册 */
  IParser(int registerPos);

  /* ParseAt 调用 CheckAvailableToParse 和 ParseAtRaw */
  /* ParseAtRaw 不需要检查是否合法 */
  IMessage* ParseAt(const TcpData* pTcpData, int pos);

  /* 此函数只需要检查头标签是否正确 */
  virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) = 0;

 private:
  virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) = 0;
};
