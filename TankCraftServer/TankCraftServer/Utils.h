#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	static long long GetRandLongLong();                                                     /* ������������� */
	static unsigned short GetUnsignedShort(const char* charArr, int pos);                   /* ��ȡ�޷��Ŷ����� */
	static void DumpUnsignedShortToBuffer(char* buffer, int pos, unsigned short val);       /* װ�ض������������� */
	static void DumpTcpDataToBuffer(char* buffer, int pos, const TcpData* tcpData);         /* �������װ����һ�������� */
	static void GetSanityInteger(const char* buffer, int len, char* evenAns, char* oddAns); /* �������� */
	static double GetClockTime();                                                           /* ��ȡ��������ǰʱ�� (���������˶�����) */
	static std::wstring GetWstringFromCharBuffer(const char* buf, int charCnt);             /* charCnt ������ż�� */
	static void DumpRawStringToBuffer(char* buf, int pos, const char* raw, int len);        /* �ַ������� */
	static void DumpIntToBuffer(char* buf, int pos, int val);                               /* �������� */
	static double GetRandomDouble();                                                        /* �������ʵ�� */
};
