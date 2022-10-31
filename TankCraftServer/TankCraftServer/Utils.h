#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	static long long GetRandLongLong();                                   /* 生成随机的整数 */
	static unsigned short GetUnsignedShort(const char* charArr, int pos); /* 获取无符号短整数 */
};
