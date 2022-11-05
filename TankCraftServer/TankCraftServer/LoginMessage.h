#pragma once
#include "IMessage.h"

#define LOGIN_STATUS_SUCCESS (0)
#define LOGIN_STATUS_SERVER_FULL (1)
#define LOGIN_STATUS_NAME_DUPLICATE (2)
#define LOGIN_STATUS_IP_BANNED (3)
#define LOGIN_STATUS_UNKNOWN_ERROR (4)

#define LOGIN_MESSAGE_LENGTH (10)  // can not be changed

class LoginMessage : public IMessage {
 public:
  LoginMessage();

  void SetStatus(unsigned short nLoginStatus);
  void SerUserId(int nUserID);

  int GetStatus() const;                     /* 获取错误码 */
  std::wstring GetStatusDescription() const; /* 获取错误码的文字描述 */

  virtual void GetRawData(TcpData* tcpData) override; /* 计算得到二进制数据 */
  virtual int GetRawDataLength() override; /* 根据数据长度定位下一个包 */
  virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
      override; /* 算法逻辑核心：根据数据修改程序存储的信息 */

  virtual void DebugShow() override;

 private:
  unsigned short mLoginStatus;
  int mUserID;
};
