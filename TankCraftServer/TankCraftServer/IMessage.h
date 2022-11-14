#pragma once
#include <vector>

class TcpData;
class IMessage;
typedef std::vector<IMessage*> IMessageList;

class ThreadBuffer; /* ÿ���ͻ����ڷ������˶�Ӧ��ʵ�� */
class GameDatabase; /* ��Ϸ������ݿ� */

class IMessage {
 public:
  virtual ~IMessage(){};

  virtual void GetRawData(TcpData* tcpData) = 0; /* ����õ����������� */
  virtual int GetRawDataLength() = 0; /* �������ݳ��ȶ�λ��һ���� */
  virtual void Dispatch(
      ThreadBuffer* tb,
      GameDatabase* Gdb) = 0; /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

  virtual void DebugShow() = 0; /* ����ר����� */
};
