#pragma once
#include "IMessage.h"

#define PING_MESSAHE_LENGTH (6)

class PingMessage : public IMessage {
 public:
  PingMessage();

  virtual int GetRawDataLength(); /* 需要根据数据长度定位下一个包 */
  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb); /* 算法逻辑核心：根据数据修改程序存储的信息 */

  void SetX(unsigned short nX); /* 设置 X 的值 */
  unsigned short GetX() const;

  virtual void GetRawData(TcpData* tcpData); /* 获取二进制数据 */
  virtual void DebugShow() override;         /* 调试专用输出 */

 private:
  unsigned short mX;
};

typedef PingMessage PingRequest; /* PingRequest 是用户发起的请求，PingMessage
                                    是服务器给出的响应 */
