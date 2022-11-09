#include "ParserRegister.h"

#include "KeyStatusRequestParser.h"
#include "LoginRequestParser.h"
#include "MapRequestParser.h"
#include "PingParser.h"
#include "UserInfoRequestParser.h"

void ParserRegister::Register() {
  /* 0 ����Ϣ��Ping ֵ���� */
  static PingParser* registerParser = new PingParser;

  /* 1 ����Ϣ����¼ע�� */
  static LoginRequestParser* registerLoginParser = new LoginRequestParser;

  /* 2 ����Ϣ�������ͼ */
  static MapRequestParser* registerMapReqeustParser = new MapRequestParser;

  /* 3 ����Ϣ���û���Ϣ���� */
  static UserInfoRequestParser* registerUserInfoRequestParser =
      new UserInfoRequestParser;

  /* 6 ����Ϣ���޸İ���״̬ */
  static KeyStatusRequestParser* registerKeyStatusRequestParser =
      new KeyStatusRequestParser();
}
