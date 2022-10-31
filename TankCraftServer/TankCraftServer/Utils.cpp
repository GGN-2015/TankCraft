#include <cstdlib>
#include <iostream>
#include <string.h>

#include "Utils.h"
#define UNSIGNED(c) ((unsigned int)(*(unsigned char*)&c))

long long Utils::GetRandLongLong()
{
	long long ans = 0;
	for (int i = 1; i <= 4; i++) {
		ans = (ans << 15) | rand(); /* 此代码只在 windows 平台可用 */
	}
	return ans;
}

unsigned short Utils::GetUnsignedShort(const char* charArr, int pos)
{
	/* 所有的数据采用小端存储 */
	unsigned short ans = UNSIGNED(charArr[pos]) | ((UNSIGNED(charArr[pos+1]) << 8));
	return ans;
}

void Utils::DumpUnsignedShortToBuffer(char* buffer, int pos, unsigned short val)
{
	unsigned char p0 = val & (0b11111111); // 低八位
	unsigned char p1 = (val >> 8);         // 高八位

	buffer[pos + 0] = *((char*)&p0);
	buffer[pos + 1] = *((char*)&p1);
}

void Utils::DumpTcpDataToBuffer(char* buffer, int pos, const TcpData* tcpData)
{
	for (int i = 0; i < tcpData->GetLength(); i += 1) {
		buffer[pos + i] = tcpData->GetData()[i];
	}
}

void Utils::GetSanityInteger(const char* buffer, int len, char* evenAns, char* oddAns)
{
	*evenAns = 0;
	*oddAns = 0;
	for (int i = 0; i < len; i += 1) {
		if ((i & 1) == 0) (*evenAns) ^= buffer[i];
		else              (*oddAns ) ^= buffer[i];
	}
}
