#pragma once
#include "IMessage.h"
class TankPosMessage : public IMessage {
 public:
  TankPosMessage(const TcpData* mapData);
  virtual ~TankPosMessage();

  virtual void GetRawData(TcpData* tcpData) override; /* ����õ����������� */
  virtual int GetRawDataLength() override; /* �������ݳ��ȶ�λ��һ���� */
  virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb)
      override; /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

  virtual void DebugShow() override;

 private:
  TcpData* mTcpData;
};
