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
