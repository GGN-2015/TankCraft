#include <cstdlib>
#include "Utils.h"

#ifndef _WIN32
#error "This Code Can Only Run On _Win32 Platform."
#endif
#include <string.h>

long long Utils::GetRandLongLong()
{
	long long ans = 0;
	for (int i = 1; i <= 4; i++) {
		ans = (ans << 15) | rand(); /* �˴���ֻ�� windows ƽ̨���� */
	}
	return ans;
}
