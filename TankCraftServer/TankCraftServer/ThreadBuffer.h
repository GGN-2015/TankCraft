#pragma once
#include "IMessage.h"

/* �����ʵ�ֲ���Ҫ�����̰߳�ȫ����Ϊ���Ǳ�֤ÿ���̸߳�����һ�� ThreadBuffer */
class ThreadBuffer
{
public:
	ThreadBuffer();

	void DumpMessage(IMessage* iMessage); /* �򻺳����л���һ����Ϣ */
	void ClearDumpedMessage();            /* ������д����͵���Ϣ */

	void GetTcpDataFromDumpedMessage(TcpData* pTcpDataGet); /* ��ȡҪ�����͵� TcpData */
	void SetUserID(int nID);                                /* ��¼���ͨ��ʱ����û� ID */

	bool InGame() const; /* ��⵱ǰ�ÿͻ��Ƿ������� */
	int GetUserID() const; /* ��ȡ�û���� */

private:
	IMessageList mIMessageList; /* �����͵���Ϣ���У������������������������ʱ���������ݽ����ͷ� */

	/* ������ǰ����Ƿ��Ѿ�������Ϸ */
	/* δ�������ҿ���ͨ����¼���������Ϸ����¼����ʧ�ܵ���Ȼ���ܼ�����Ϸ */
	/* �ս������ӵ��û����ڹ�սģʽ��ID = -1�����ܲ���̹�� */
	/* �Ѿ���¼���û��ٴε�¼������Ϣ�ᱻ���� */
	int mUserID;

	double mLastKillListTime; /* ��һ�λ�ȡ KillList ��ʱ�� */
	double mLastShootTime;    /* �ϴη����ڵ���ʱ�� */
};
