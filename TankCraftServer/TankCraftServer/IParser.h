#pragma once
#include <vector>

class TcpData;
class IMessage;
class IParser;

#define IPARSER_REGISTER_AT_CLINET_PARSER \
  (0) /* ������ע�ᵽ�ͻ�������������� */
#define IPARSER_REGISTER_AT_SERVER_PARSER \
  (1) /* ������ע�ᵽ������Ϣ����������*/

typedef std::vector<IParser*> IParserList; /* �������б� */

class IParser {
 public:
  /* ÿ�� IParser ����ʵ������ʱ���Զ�ע�� */
  IParser(int registerPos);

  /* ParseAt ���� CheckAvailableToParse �� ParseAtRaw */
  /* ParseAtRaw ����Ҫ����Ƿ�Ϸ� */
  IMessage* ParseAt(const TcpData* pTcpData, int pos);

  /* �˺���ֻ��Ҫ���ͷ��ǩ�Ƿ���ȷ */
  virtual bool CheckAvailableToParse(const TcpData* pTcpData, int pos) = 0;

 private:
  virtual IMessage* ParseAtRaw(const TcpData* pTcpData, int pos) = 0;
};
