#pragma once
#include <string>

#include "IMessage.h"

class LoginRequest : public IMessage {
 public:
  virtual int GetRawDataLength(); /* ��Ҫ�������ݳ��ȶ�λ��һ���� */
  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb); /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

  void SetUserName(const std::wstring& nUserName); /* �����û��� */
  std::wstring GetUserName() const;

  virtual void GetRawData(TcpData* tcpData); /* ��ȡ���������� */
  virtual void DebugShow() override;         /* ����ר����� */

 private:
  std::wstring mUserName;
};
