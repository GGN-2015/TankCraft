#pragma once
#include "IMessage.h"
class UserInfoRequest :
    public IMessage
{
public:
	virtual int  GetRawDataLength();                            /* ��Ҫ�������ݳ��ȶ�λ��һ���� */
	virtual void Dispatch(ThreadBuffer* tb, GameDatabase* Gdb); /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */

	virtual void GetRawData(TcpData* tcpData); /* ��ȡ���������� */
	virtual void DebugShow() override;         /* ����ר����� */
};
