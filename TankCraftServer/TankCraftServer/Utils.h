#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	static long long GetRandLongLong();                                   /* ������������� */
	static unsigned short GetUnsignedShort(const char* charArr, int pos); /* ��ȡ�޷��Ŷ����� */
};
