#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "Utils.h"
#define UNSIGNED(c) (*(unsigned char*)&c)

long long Utils::GetRandLongLong()
{
	long long ans = 0;
	for (int i = 1; i <= 4; i++) {
		ans = (ans << 15) | rand(); /* �˴���ֻ�� windows ƽ̨���� */
	}
	return ans;
}

unsigned short Utils::GetUnsignedShort(const char* charArr, int pos)
{
	/* ���е����ݲ���С�˴洢 */
	unsigned short ans = *(unsigned short*)&charArr[pos];
	return ans;
}

void Utils::DumpUnsignedShortToBuffer(char* buffer, int pos, unsigned short val)
{
	unsigned char p0 = val & (0b11111111); // �Ͱ�λ
	unsigned char p1 = (val >> 8);         // �߰�λ

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

double Utils::GetClockTime()
{
	return 1.0 * clock() / CLOCKS_PER_SEC;
}

std::wstring Utils::GetWstringFromCharBuffer(const char* buf, int charCnt)
{
	assert(charCnt % 2 == 0);
	int wcharCnt = charCnt / 2;

	wchar_t* wbuf = new wchar_t[wcharCnt + 1];
	memcpy(wbuf, buf, charCnt);
	wbuf[wcharCnt] = '\0'; /* ���һ�� '\0' */

	std::wstring ans = wbuf; /* ת�����ַ��� */
	delete[] wbuf;

	return ans;
}

void Utils::DumpRawStringToBuffer(char* buf, int pos, const char* raw, int len)
{
	for (int i = 0; i < len; i += 1) {
		buf[pos + i] = raw[i];
	}
}

void Utils::DumpIntToBuffer(char* buf, int pos, int val)
{
	unsigned int uVal = *(unsigned int*)&val;
	unsigned char p[4];
	for (int i = 0; i < 4; i += 1) {
		p[i] = uVal & (0b11111111); /* ��ȡ�ӵ͵����ĸ��ֽ� */
		uVal >>= 8;
	}

	for (int i = 0; i < 4; i += 1) {
		UNSIGNED(buf[pos + i]) = p[i]; /* ����д�� */
	}
}
