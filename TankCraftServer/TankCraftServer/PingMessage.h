#pragma once
#include "IMessage.h"

#define PING_MESSAHE_LENGTH (4)

class PingMessage :
    public IMessage
{
public:
	PingMessage();

	virtual int  GetRawDataLength();                            /* �������ݳ��ȶ�λ��һ���� */
	virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb); /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

	void SetX(unsigned short nX); /* ���� X ��ֵ */
	unsigned short GetX() const;

	virtual void GetRawData(TcpData* tcpData); /* ��ȡ���������� */

private:
	unsigned short mX;
};
