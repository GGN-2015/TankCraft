#pragma once
#include "IMessage.h"

#define PING_MESSAHE_LENGTH (6)

class PingMessage : public IMessage {
 public:
  PingMessage();

  virtual int GetRawDataLength(); /* ��Ҫ�������ݳ��ȶ�λ��һ���� */
  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb); /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

  void SetX(unsigned short nX); /* ���� X ��ֵ */
  unsigned short GetX() const;

  virtual void GetRawData(TcpData* tcpData); /* ��ȡ���������� */
  virtual void DebugShow() override;         /* ����ר����� */

 private:
  unsigned short mX;
};

typedef PingMessage PingRequest; /* PingRequest ���û����������PingMessage
                                    �Ƿ�������������Ӧ */
