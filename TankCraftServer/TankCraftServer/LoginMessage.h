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

  int GetStatus() const;                     /* ��ȡ������ */
  std::wstring GetStatusDescription() const; /* ��ȡ��������������� */

  virtual void GetRawData(TcpData* tcpData) override; /* ����õ����������� */
  virtual int GetRawDataLength() override; /* �������ݳ��ȶ�λ��һ���� */
  virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
      override; /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

  virtual void DebugShow() override;

 private:
  unsigned short mLoginStatus;
  int mUserID;
};
