#pragma once
#include <vector>

class TcpData;
class IMessage;
typedef std::vector<IMessage*> IMessageList;

class ThreadBuffer; /* 每个客户端在服务器端对应的实体 */
class GameDatabase; /* 游戏玩家数据库 */

class IMessage {
 public:
  virtual ~IMessage(){};

  virtual void GetRawData(TcpData* tcpData) = 0; /* 计算得到二进制数据 */
  virtual int GetRawDataLength() = 0; /* 根据数据长度定位下一个包 */
  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb) = 0; /* 算法逻辑核心：根据数据修改程序存储的信息 */

  virtual void DebugShow() = 0; /* 调试专用输出 */
};
