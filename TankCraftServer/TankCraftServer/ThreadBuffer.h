#pragma once
#include "IMessage.h"

/* �����ʵ�ֲ���Ҫ�����̰߳�ȫ����Ϊ���Ǳ�֤ÿ���̸߳�����һ�� ThreadBuffer */
class ThreadBuffer
{
public:
	void DumpMessage(IMessage* iMessage); /* �򻺳����л���һ����Ϣ */
	void ClearDumpedMessage();            /* ������д����͵���Ϣ */

	void GetTcpDataFromDumpedMessage(TcpData* pTcpDataGet); /* ��ȡҪ�����͵� TcpData */

private:
	IMessageList mIMessageList; /* �����͵���Ϣ���У������������������������ʱ���������ݽ����ͷ� */
};
