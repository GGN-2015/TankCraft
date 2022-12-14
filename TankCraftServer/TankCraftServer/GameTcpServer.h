#pragma once
#include "TcpServer.h"

#define GAME_TCP_SERVER_DEBUG

class TcpData;
class GameTcpServer : public TcpServer {
 public:
  GameTcpServer(const char* ip, int port); /* 创建游戏服务器 */

  virtual void GetTcpDataResult(const TcpData* pTcpDataRecv,
                                TcpData* pTcpDataSend, ThreadBuffer* tb,
                                GameDatabase* Gdb) override;
};
