#pragma once
#include "IMessage.h"
class UserInfoMessage : public IMessage {
 public:
  UserInfoMessage(const TcpData* mapData);
  ~UserInfoMessage();

  virtual void GetRawData(TcpData* tcpData) override; /* 计算得到二进制数据 */
  virtual int GetRawDataLength() override; /* 根据数据长度定位下一个包 */
  virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
      override; /* 算法逻辑核心：根据数据修改程序存储的信息 */

  virtual void DebugShow() override;

 private:
  TcpData* mTcpData;
};
