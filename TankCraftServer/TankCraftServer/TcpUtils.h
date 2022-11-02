#pragma once
#include "TankCraft_NetManager_Component.h"

#define TCP_BUFFER_MAX (1048576) /* TCP �Ļ�������С */

#define TCP_SERVER_CLIENT_MAX  (16) /* ��������� */

#define TCP_SERVER_RUN (1) /* �������������� */
#define TCP_SERVER_END (0) /* �������������� */

#define TCP_SERVER_SUC        ( 0) /* �������ɹ�ͣ�� */
#define TCP_SERVER_FAIL       ( 1) /* ����������ʧ�� */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP ���������������� */

#define CLIENT_THREAD_SLEEP_TIME (60) /* �ͻ��˷�����Ϣ������ ms for windows */

class TcpServer;
class TcpData;
class Xn::TankCraft::NetManager_Component;
class Xn::TankCraft::NetMessageBaseDataList;

class TcpUtils
{
public:
	/* static ����ʵ��һЩ��Ҫʹ�� SOCKET �Ĺ��ܷ�װ */
	static void DealWithClient(void* socketClient, TcpServer* tcpServer);
	static void GetTcpDataFromSocket(void* socketClient, TcpData* tcpData);
	static int  SendTcpDataToSocket(const TcpData* tcpData, void* socketClient); /* ���ܷ��� -1 */
	static void CreateProcessForClient(void* socketClient, TcpServer* tcpServer);

	/* �ͻ��˽������ݵ��߳� */
	static void ClientThreadFunction(const char* ip, int port, Xn::TankCraft::NetManager_Component* nmComponent);
	
	/* ����� Request ��Ϣ�����һ�� Request ���ݰ� */
	static void CompactTcpDataListToTcpDataRequest(const TcpDataList* tcpDataList, TcpData* tcpDataRequest);

	/* ��һ�����ݰ���ֳɶ�� Message*/
	static void UnpackTcpDataMessageToTcpDataList(const TcpData* pTcpDataMessage, TcpDataList* pTcpDataList);

	/* �� tcpDataList ת���� nmBaseDataList */
	static void GetTcpDataListFromNetMessageBaseDataList(Xn::TankCraft::NetMessageBaseDataList* nmBaseDataList, TcpDataList* tcpDataList);
};
