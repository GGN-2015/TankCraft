#pragma once
#include "IMessage.h"
class KeyStatusRequest : public IMessage {
 public:
  virtual void GetRawData(TcpData* tcpData); /* 计算得到二进制数据 */
  virtual int GetRawDataLength(); /* 根据数据长度定位下一个包 */

  void SetKeyId(unsigned char nKeyId);
  void SetKeyStatus(unsigned char nKeyStatus); /* 设置信息 */

  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb); /* 算法逻辑核心：根据数据修改程序存储的信息 */

  virtual void DebugShow(); /* 调试专用输出 */
 private:
  unsigned char mKeyId, mKeyStatus;
};
