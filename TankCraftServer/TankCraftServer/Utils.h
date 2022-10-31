#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	static long long GetRandLongLong();                                               /* 生成随机的整数 */
	static unsigned short GetUnsignedShort(const char* charArr, int pos);             /* 获取无符号短整数 */
	static void DumpUnsignedShortToBuffer(char* buffer, int pos, unsigned short val); /* 装载短整数到二进制 */
	static void DumpTcpDataToBuffer(char* buffer, int pos, const TcpData* tcpData);   /* 向二进制装载另一个二进制 */
	static void GetSanityInteger(const char* buffer, int len, char* evenAns, char* oddAns); /* 计算异或和 */
};
