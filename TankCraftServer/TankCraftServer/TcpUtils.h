#pragma once

#define TCP_BUFFER_MAX (1048576) /* TCP �Ļ�������С */

#define TCP_SERVER_CLIENT_MAX  (16) /* ��������� */

#define TCP_SERVER_RUN (1) /* �������������� */
#define TCP_SERVER_END (0) /* �������������� */

#define TCP_SERVER_SUC        ( 0) /* �������ɹ�ͣ�� */
#define TCP_SERVER_FAIL       ( 1) /* ����������ʧ�� */
#define TCP_SERVER_MAX_LISTEN (10) /* TCP ���������������� */

class TcpServer;
class TcpData;
class TcpUtils
{
public:
	/* static ����ʵ��һЩ��Ҫʹ�� SOCKET �Ĺ��ܷ�װ */
	static void DealWithClient(void* socketClient, TcpServer* tcpServer);
	static void GetTcpDataFromSocket(void* socketClient, TcpData* tcpData);
	static int  SendTcpDataToSocket(const TcpData* tcpData, void* socketClient); /* ���ܷ��� -1 */
	static void CreateProcessForClient(void* socketClient, TcpServer* tcpServer);
};
