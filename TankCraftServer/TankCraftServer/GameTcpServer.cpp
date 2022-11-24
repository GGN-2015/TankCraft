#include "GameTcpServer.h"

#include <vector>

#include "DataParser.h"
#include "ParserRegister.h"
#include "TcpData.h"
#include "ThreadBuffer.h"

GameTcpServer::GameTcpServer(const char* ip, int port) : TcpServer(ip, port) {
  static ParserRegister* pr = nullptr;

  /* 注册请求解析器列表 */
  if (pr == nullptr) {
    pr = new ParserRegister;
    pr->Register();
  }
}

void GameTcpServer::GetTcpDataResult(const TcpData* pTcpDataRecv,
                                     TcpData* pTcpDataSend, ThreadBuffer* tb,
                                     GameDatabase* Gdb) {
  /* 使用数据解析器将消息序列解析成多种多样的消息 */
  DataParser dataParser;
  IMessageList iClientRequestList;
  dataParser.Parse(pTcpDataRecv, &iClientRequestList,
                   DataParser::GetGlobalClientRequestParserList());

  /* 根据消息内容做出改变，或者派发消息 */
  for (auto iClientRequest : iClientRequestList) {
    iClientRequest->Dispatch(tb, Gdb);
  }

  /* shared_ptr 不需要考虑释放问题 */
  /* 完成工作后要记得释放收到的 Request 数据 */
  // for (auto iClientRequest : iClientRequestList) {
  //   delete iClientRequest;
  // }

  /* 在这里加入所有周期性检查事件 */
  tb->DumpTankPosMessage(Gdb);   /* 获取并发送坦克位置信息 */
  tb->DumpBulletPosMessage(Gdb); /* 获取并发送子弹位置信息 */

  tb->DumpScoreBoardMessage(Gdb); /* 获取计分板消息 */
  tb->DumpUserInfoMessage(Gdb);   /* 输出用户名称颜色映射 */

  tb->GetTcpDataFromDumpedMessage(pTcpDataSend); /* 获取要被发送的消息 */
  tb->ClearDumpedMessage(); /* 清空缓存的要发送的消息 */
}
