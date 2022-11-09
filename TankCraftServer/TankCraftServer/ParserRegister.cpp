#include "ParserRegister.h"

#include "KeyStatusRequestParser.h"
#include "LoginRequestParser.h"
#include "MapRequestParser.h"
#include "PingParser.h"
#include "UserInfoRequestParser.h"

void ParserRegister::Register() {
  /* 0 号消息：Ping 值计算 */
  static PingParser* registerParser = new PingParser;

  /* 1 号消息：登录注册 */
  static LoginRequestParser* registerLoginParser = new LoginRequestParser;

  /* 2 号消息：请求地图 */
  static MapRequestParser* registerMapReqeustParser = new MapRequestParser;

  /* 3 号消息：用户信息请求 */
  static UserInfoRequestParser* registerUserInfoRequestParser =
      new UserInfoRequestParser;

  /* 6 号消息：修改按键状态 */
  static KeyStatusRequestParser* registerKeyStatusRequestParser =
      new KeyStatusRequestParser();
}
