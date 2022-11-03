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

	bool HasGraphTcpDataCache() const; /* ����Ƿ��л���ĵ�ͼ���� */

	void DumpGraphTcpDataIntoMessageList(GameDatabase* Gdb);
	void SetGraphTimer(); /* ��¼��ǰʱ����Ϊ���µ�ͼ��׼�� */

	bool GraphTcpDataCacheOutofData(GameDatabase* Gdb) const; /* ��⵱ǰ�ĵ�ͼ�Ƿ���� */
	void FreeGraphTcpDataCache(); /* ��ջ���ĵ�ͼ��Ϣ */

private:

	IMessageList mIMessageList; /* �����͵���Ϣ���У������������������������ʱ���������ݽ����ͷ� */

	/* ������ǰ����Ƿ��Ѿ�������Ϸ */
	/* δ�������ҿ���ͨ����¼���������Ϸ����¼����ʧ�ܵ���Ȼ���ܼ�����Ϸ */
	/* �ս������ӵ��û����ڹ�սģʽ��ID = -1�����ܲ���̹�� */
	/* �Ѿ���¼���û��ٴε�¼������Ϣ�ᱻ���� */
	int mUserID;

	double mLastKillListTime; /* ��һ�λ�ȡ KillList ��ʱ�� */
	double mLastShootTime;    /* �ϴη����ڵ���ʱ�� */
	double mLastGetGraphTime; /* �ϴλ�ȡ��ͼ��ʱ�� */

	TcpData* mGraphTcpDataCache = nullptr;
};
