#pragma once
#include "TcpData.h"
#include "TcpServer.h"

class Utils
{
public:

	static long long GetRandLongLong();                                                     /* 生成随机的整数 */
	static unsigned short GetUnsignedShort(const char* charArr, int pos);                   /* 获取无符号短整数 */
	static void DumpUnsignedShortToBuffer(char* buffer, int pos, unsigned short val);       /* 装载短整数到二进制 */
	static void DumpTcpDataToBuffer(char* buffer, int pos, const TcpData* tcpData);         /* 向二进制装载另一个二进制 */
	static void GetSanityInteger(const char* buffer, int len, char* evenAns, char* oddAns); /* 计算异或和 */
	static double GetClockTime();                                                           /* 获取服务器当前时刻 (开机运行了多少秒) */
	static std::wstring GetWstringFromCharBuffer(const char* buf, int charCnt);             /* charCnt 必须是偶数 */
	static void DumpRawStringToBuffer(char* buf, int pos, const char* raw, int len);        /* 字符串载入 */
	static void DumpIntToBuffer(char* buf, int pos, int val);                               /* 整数载入 */
	static double GetRandomDouble();                                                        /* 生成随机实数 */
};
