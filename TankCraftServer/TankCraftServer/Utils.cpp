#include <cstdlib>
#include <iostream>
#include <string.h>

#include "Utils.h"

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
	unsigned short ans = (unsigned)charArr[pos] | (((unsigned)charArr[pos+1]) << 8);
	return ans;
}
