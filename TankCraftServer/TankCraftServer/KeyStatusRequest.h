#pragma once
#include "IMessage.h"
class KeyStatusRequest : public IMessage {
 public:
  virtual void GetRawData(TcpData* tcpData); /* ����õ����������� */
  virtual int GetRawDataLength(); /* �������ݳ��ȶ�λ��һ���� */

  void SetKeyId(unsigned char nKeyId);
  void SetKeyStatus(unsigned char nKeyStatus); /* ������Ϣ */

  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb); /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

  virtual void DebugShow(); /* ����ר����� */
 private:
  unsigned char mKeyId, mKeyStatus;
};
