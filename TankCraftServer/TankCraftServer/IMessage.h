#pragma once
class IMessage
{
public:
	virtual int  GetRawDataLength() = 0; /* �������ݳ��ȶ�λ��һ���� */
	virtual void Dispatch() = 0;         /* �㷨�߼����ģ����������޸ĳ���洢����Ϣ */
};
