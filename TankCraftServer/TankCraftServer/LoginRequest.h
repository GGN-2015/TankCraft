#pragma once
#include <string>

#include "IMessage.h"

class LoginRequest : public IMessage {
 public:
  virtual int GetRawDataLength(); /* 需要根据数据长度定位下一个包 */
  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb); /* 算法逻辑核心：根据数据修改程序存储的信息 */

  void SetUserName(const std::wstring& nUserName); /* 设置用户名 */
  std::wstring GetUserName() const;

  virtual void GetRawData(TcpData* tcpData); /* 获取二进制数据 */
  virtual void DebugShow() override;         /* 调试专用输出 */

 private:
  std::wstring mUserName;
};
